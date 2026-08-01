package com.zeroone.star.calendar.service.impl;

import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.calendar.entity.WkfSlot;
import com.zeroone.star.calendar.mapper.WkfSlotMapper;
import com.zeroone.star.calendar.service.ISlotCalendarService;
import com.zeroone.star.project.dto.j8.calendar.SlotBlockDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDetailDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotOverbookDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotPageResultDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotStatsDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotUnblockDTO;
import com.zeroone.star.project.query.j8.calendar.SlotQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * <p>
 * 号源日历 服务实现
 * </p>
 * @author j8组
 * @since 2026-07-26
 */
@Service
public class SlotCalendarServiceImpl extends ServiceImpl<WkfSlotMapper, WkfSlot>
        implements ISlotCalendarService {

    /** 封锁状态码 */
    private static final String STATUS_BUSY_UNAVAILABLE = "busy-unavailable";
    /** 可约状态码 */
    private static final String STATUS_FREE = "free";

    @Override
    public JsonVO<SlotPageResultDTO> pageSlots(SlotQuery query) {
        // 1. 构建分页对象
        Page<SlotDTO> page = new Page<>(query.getPageNo(), query.getPageSize());

        // 2. 执行分页查询
        Page<SlotDTO> result = baseMapper.selectSlotPage(page, query);

        // 3. 计算 displayStatus + aggregate stats
        List<SlotDTO> rows = result.getRecords();
        int totalSlots = 0;
        int totalCapacity = 0;
        int totalBooked = 0;
        int totalRemaining = 0;

        for (SlotDTO row : rows) {
            row.setDisplayStatus(computeDisplayStatus(row));
            totalSlots++;
            int capacity = row.getCapacity() != null ? row.getCapacity() : 0;
            int booked = row.getBookedCount() != null ? row.getBookedCount() : 0;
            totalCapacity += capacity;
            totalBooked += booked;
            totalRemaining += (capacity - booked);
        }

        // 4. 组装 SlotStatsDTO
        SlotStatsDTO stats = new SlotStatsDTO();
        stats.setTotalSlots(totalSlots);
        stats.setTotalCapacity(totalCapacity);
        stats.setTotalBooked(totalBooked);
        stats.setTotalRemaining(totalRemaining);

        // 5. 组装 SlotPageResultDTO
        SlotPageResultDTO pageResult = new SlotPageResultDTO();
        pageResult.setTotalCount(result.getTotal());
        pageResult.setPageNo(result.getCurrent());
        pageResult.setPageSize(result.getSize());
        pageResult.setPages(result.getPages());
        pageResult.setContent(rows);
        pageResult.setStats(stats);

        return JsonVO.success(pageResult);
    }

    @Override
    public JsonVO<SlotDetailDTO> getSlotDetail(String slotId) {
        SlotDetailDTO detail = baseMapper.selectSlotDetail(slotId);
        if (detail == null) {
            return JsonVO.fail("号源不存在");
        }
        detail.setDisplayStatus(computeDisplayStatus(detail));
        return JsonVO.success(detail);
    }

    @Override
    public JsonVO<Void> blockSlots(SlotBlockDTO dto) {
        // 先查这些号源是否确实存在
        long count = this.lambdaQuery()
                .in(WkfSlot::getId, dto.getSlotIds())
                .count();
        if (count == 0) {
            return JsonVO.fail("号源不存在");
        }

        // 只更新当前非封锁的号源（已封锁的不算失败），同时记录封锁原因
        LambdaUpdateWrapper<WkfSlot> wrapper = new LambdaUpdateWrapper<>();
        wrapper.in(WkfSlot::getId, dto.getSlotIds())
               .ne(WkfSlot::getStatusCode, STATUS_BUSY_UNAVAILABLE)
               .set(WkfSlot::getStatusCode, STATUS_BUSY_UNAVAILABLE);
        if (dto.getReason() != null && !dto.getReason().isEmpty()) {
            wrapper.set(WkfSlot::getCommentText, dto.getReason());
        }
        this.update(wrapper);
        return JsonVO.success(null);
    }

    @Override
    public JsonVO<Void> unblockSlots(SlotUnblockDTO dto) {
        // 先查这些号源是否确实存在（排除已软删的）
        long count = this.lambdaQuery()
                .in(WkfSlot::getId, dto.getSlotIds())
                .count();
        if (count == 0) {
            return JsonVO.fail("号源不存在");
        }

        // 只更新当前状态为封锁的号源（已经是 free 的不算失败）
        LambdaUpdateWrapper<WkfSlot> wrapper = new LambdaUpdateWrapper<>();
        wrapper.in(WkfSlot::getId, dto.getSlotIds())
               .eq(WkfSlot::getStatusCode, STATUS_BUSY_UNAVAILABLE)
               .set(WkfSlot::getStatusCode, STATUS_FREE);
        this.update(wrapper);
        return JsonVO.success(null);
    }

    @Override
    public JsonVO<Void> setOverbooked(SlotOverbookDTO dto) {
        Integer targetFlag = dto.getOverbookedFlag() ? 1 : 0;

        WkfSlot slot = this.getById(dto.getSlotId());
        if (slot == null) {
            return JsonVO.fail("号源不存在");
        }
        // 已经是目标状态，无需更新
        if (targetFlag.equals(slot.getOverbookedFlag())) {
            return JsonVO.success(null);
        }

        slot.setOverbookedFlag(targetFlag);
        this.updateById(slot);
        return JsonVO.success(null);
    }

    /**
     * 根据号源数据计算展示状态
     * <ul>
     *   <li>封锁（statusCode=busy-unavailable）→ "封锁"</li>
     *   <li>超售（overbookedFlag=1 且 已约>=容量）→ "超售"</li>
     *   <li>已满（已约>=容量且未开超售）→ "已满"</li>
     *   <li>可约（已约<容量）→ "可约"</li>
     * </ul>
     */
    private String computeDisplayStatus(SlotDTO row) {
        if (STATUS_BUSY_UNAVAILABLE.equals(row.getStatusCode())) {
            return "封锁";
        }
        int capacity = row.getCapacity() != null ? row.getCapacity() : 0;
        int booked = row.getBookedCount() != null ? row.getBookedCount() : 0;
        boolean overbooked = Boolean.TRUE.equals(row.getOverbookedFlag());

        if (booked >= capacity) {
            return overbooked ? "超售" : "已满";
        }
        return "可约";
    }
}
