package com.zeroone.star.clinics.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.clinics.entity.WkfSchedule;
import com.zeroone.star.clinics.entity.WkfSlot;
import com.zeroone.star.clinics.mapper.WkfScheduleMapper;
import com.zeroone.star.clinics.mapper.WkfSlotMapper;
import com.zeroone.star.clinics.service.IWkfSlotService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j8.clinics.RetireScheduleDTO;
import com.zeroone.star.project.dto.j8.clinics.SlotBatchReleaseDTO;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.j8.clinics.ResumeScheduleVO;
import com.zeroone.star.project.vo.j8.clinics.SlotBatchReleaseVO;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * <p>
 * 号源时段（FHIR 时段 语义） 服务实现类
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Slf4j
@Service
public class WkfSlotServiceImpl extends ServiceImpl<WkfSlotMapper, WkfSlot> implements IWkfSlotService {

    @Resource
    private WkfScheduleMapper scheduleMapper;

    @Resource
    private UserHolder userHolder;

    /**
     * 获取已放号源列表
     * 多表联查 wkf_slot + wkf_schedule + wkf_schedule_actor + adm_practitioner + adm_healthcare_service + adm_organization
     *
     * @param query 查询参数
     * @return 放号源列表
     */
    @Override
    public List<SlotVO> getRelNumberSource(RelSlotQuery query) {
        return baseMapper.selectRelSlots(query);
    }

    /**
     * 删除号源
     *
     * @param slotId 待删除的号源时段主键id
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void deleteNumberSource(String slotId) {
        LambdaQueryWrapper<WkfSlot> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfSlot::getId, slotId);
        WkfSlot slot = this.getOne(wrapper);
        if (slot == null) {
            throw new RuntimeException("待删除的号源不存在");
        }
        if (!this.remove(wrapper)) {
            throw new RuntimeException("删除号源失败");
        }
    }

    /**
     * 批量放号
     *
     * @param dto 批量放号参数
     * @return 批量放号结果
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public SlotBatchReleaseVO batchReleaseNumberSource(SlotBatchReleaseDTO dto) {
        List<WkfSlot> slots = new java.util.ArrayList<>();
        for (var item : dto.getSlotTemplates()) {
            WkfSlot slot = new WkfSlot();
            slot.setOrganizationId(dto.getOrganizationId());
            slot.setScheduleId(dto.getScheduleId());
            slot.setStartTime(item.getStartTime());
            slot.setEndTime(item.getEndTime());
            slot.setCapacity(item.getCapacity());
            slot.setBookedCount(0);
            slot.setStatusCode("free");
            slot.setOverbookedFlag(false);
            slot.setCreatedAt(java.time.LocalDateTime.now());
            slot.setTenantId(getCurrentTenantId());
            slots.add(slot);
        }

        // 时间段冲突检测
        for (var item : dto.getSlotTemplates()) {
            long conflictCount = count(new LambdaQueryWrapper<WkfSlot>()
                    .eq(WkfSlot::getScheduleId, dto.getScheduleId())
                    .lt(WkfSlot::getStartTime, item.getEndTime())
                    .gt(WkfSlot::getEndTime, item.getStartTime()));
            if (conflictCount > 0) {
                throw new RuntimeException("时段与同科室其他日程已占用的时间窗冲突，或已在待放号列表中");
            }
        }

        try {
            saveBatch(slots);
            log.info("批量放号成功，scheduleId: {}, 放号数量: {}", dto.getScheduleId(), slots.size());
        } catch (Exception e) {
            log.error("批量放号失败，scheduleId: {}, 原因: {}", dto.getScheduleId(), e.getMessage(), e);
            throw new RuntimeException("批量放号失败：" + e.getMessage(), e);
        }

        SlotBatchReleaseVO vo = new SlotBatchReleaseVO();
        vo.setSlotIds(slots.stream().map(WkfSlot::getId).toList());
        vo.setGeneratedCount(slots.size());
        return vo;
    }




    /**
     * 恢复开诊：将停诊日程恢复为开诊状态
     *
     * @param scheduleId 排班id
     * @return 恢复开诊结果
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public ResumeScheduleVO reactivateSchedule(String scheduleId) {
        // 查询日程
        WkfSchedule schedule = scheduleMapper.selectById(scheduleId);
        if (schedule == null) {
            throw new RuntimeException("日程不存在");
        }
        if (!"inactive".equals(schedule.getStatusEnum())) {
            throw new RuntimeException("当前日程状态不是停诊，不能修改");
        }

        // 更新状态为开诊
        schedule.setStatusEnum("active");
        schedule.setActiveFlag(true);
        scheduleMapper.updateById(schedule);

        // 检查是否需要重新生成号源
        boolean hasSlots = count(new LambdaQueryWrapper<WkfSlot>()
                .eq(WkfSlot::getScheduleId, scheduleId)) > 0;

        ResumeScheduleVO vo = new ResumeScheduleVO();
        vo.setScheduleId(scheduleId);
        vo.setNeedReprovision(!hasSlots);
        return vo;
    }

    /**
     * 作废日程
     *
     *
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void retireSchedule(RetireScheduleDTO dto) {
        WkfSchedule schedule = scheduleMapper.selectById(dto.getScheduleId());
        if (schedule == null) {
            throw new RuntimeException("日程不存在");
        }
        if ("retired".equals(schedule.getStatusEnum())) {
            throw new RuntimeException("日程已作废");
        }

        schedule.setStatusEnum("retired");
        schedule.setActiveFlag(false);
        scheduleMapper.updateById(schedule);

        log.info("作废日程成功，scheduleId: {}, 原因: {}", dto.getScheduleId(), dto.getReason());
    }

    /**
     * 从当前用户上下文获取租户ID
     */
    private String getCurrentTenantId() {
        try {
            var user = userHolder.getCurrentUser();
            if (user.getOrganizationUnits() != null && !user.getOrganizationUnits().isEmpty()) {
                return user.getOrganizationUnits().keySet().iterator().next();
            }
        } catch (Exception e) {
            log.warn("获取当前用户租户信息失败，使用默认值: {}", e.getMessage());
        }
        return "1";
    }


}
