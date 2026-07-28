package com.zeroone.star.clinics.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.clinics.entity.WkfSlot;
import com.zeroone.star.clinics.mapper.WkfSlotMapper;
import com.zeroone.star.clinics.service.IWkfSlotService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.j8.clinics.SlotBatchReleaseDTO;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.j8.clinics.SlotBatchReleaseVO;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.UUID;

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
            slot.setId(UUID.randomUUID().toString().replace("-", ""));
            slot.setOrganizationId(dto.getOrganizationId());
            slot.setScheduleId(dto.getScheduleId());
            slot.setStartTime(item.getStartTime());
            slot.setEndTime(item.getEndTime());
            slot.setCapacity(item.getCapacity());
            slot.setBookedCount(0);
            slot.setStatusCode("free");
            slot.setOverbookedFlag(false);
            slot.setCreatedAt(java.time.LocalDateTime.now());
            slots.add(slot);
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


}
