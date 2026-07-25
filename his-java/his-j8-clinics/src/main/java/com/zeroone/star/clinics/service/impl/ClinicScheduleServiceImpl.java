// service/impl/ClinicScheduleServiceImpl.java
package com.zeroone.star.clinics.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.clinics.entity.*;
import com.zeroone.star.clinics.exception.BusinessException;
import com.zeroone.star.clinics.service.*;
import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;
import com.zeroone.star.project.dto.j8.clinics.AppointmentDisposalDTO;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.List;
import java.util.stream.Collectors;

/**
 * 门诊排班管理业务服务实现
 */
@Slf4j
@Service
@RequiredArgsConstructor
public class ClinicScheduleServiceImpl implements ClinicScheduleService {


    // 注入 MyBatis-Plus 生成的基础 Service
    private final IWkfScheduleService scheduleService;
    private final IWkfSlotService slotService;
    private final IWkfAppointmentQueueService appointmentQueueService;
    private final IBkgStopClinicSlotFreezeService slotFreezeService;
    private final IBkgScheduleChangePolicyService changePolicyService;
    private final IWkfScheduleActorService scheduleActorService;  // 新增：日程参与者服务

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void setSubstitute(SubstituteDTO dto) {
        log.info("设置替诊，scheduleId: {}, substitutePractitionerId: {}", dto.getScheduleId(), dto.getSubstitutePractitionerId());

        // 1. 查询日程
        String scheduleIdStr = String.valueOf(dto.getScheduleId());
        WkfSchedule schedule = scheduleService.getById(scheduleIdStr);
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 判断日程状态是否为 active
        if (!"active".equals(schedule.getStatusEnum())) {
            throw new BusinessException("仅已发布日程可替诊");
        }

        // 3. 判断替诊医师 ≠ 原医师
        String substituteIdStr = String.valueOf(dto.getSubstitutePractitionerId());
        String originalPractitionerId = getOriginalPractitionerId(scheduleIdStr);
        if (originalPractitionerId != null && substituteIdStr.equals(originalPractitionerId)) {
            throw new BusinessException("替诊医师不能与原医师相同");
        }

        // 4. 执行替诊：更新 wkf_schedule 表
        WkfSchedule updateEntity = new WkfSchedule();
        updateEntity.setId(scheduleIdStr);
        updateEntity.setSubstitutePractitionerId(substituteIdStr);
        updateEntity.setSubstituteReason(dto.getSubstituteReason());
        scheduleService.updateById(updateEntity);

        log.info("替诊设置成功，scheduleId: {}", scheduleIdStr);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void cancelSubstitute(Long scheduleId) {
        log.info("撤销替诊，scheduleId: {}", scheduleId);

        // 1. 查询日程
        String scheduleIdStr = String.valueOf(scheduleId);
        WkfSchedule schedule = scheduleService.getById(scheduleIdStr);
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 判断是否已替诊
        if (schedule.getSubstitutePractitionerId() == null) {
            throw new BusinessException("该日程未设置替诊");
        }

        // 3. 执行撤销：清空替诊信息
        WkfSchedule updateEntity = new WkfSchedule();
        updateEntity.setId(scheduleIdStr);
        updateEntity.setSubstitutePractitionerId(null);
        updateEntity.setSubstituteReason(null);
        scheduleService.updateById(updateEntity);

        log.info("替诊撤销成功，scheduleId: {}", scheduleIdStr);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void executeAllDayStopClinic(AllDayStopClinicExecuteDTO dto) {
        log.info("执行整日停诊，scheduleId: {}", dto.getScheduleId());

        // 1. 查询日程
        String scheduleIdStr = String.valueOf(dto.getScheduleId());
        WkfSchedule schedule = scheduleService.getById(scheduleIdStr);
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 查询该日程下的所有号源
        List<WkfSlot> slots = slotService.list(
                new LambdaQueryWrapper<WkfSlot>()
                        .eq(WkfSlot::getScheduleId, scheduleIdStr)
                        .eq(WkfSlot::getIsDeleted, false)
        );

        if (slots.isEmpty()) {
            throw new BusinessException("该日程下没有号源");
        }

        // 3. 获取所有 slotId
        List<String> slotIds = slots.stream()
                .map(WkfSlot::getId)
                .collect(Collectors.toList());

        // 4. 查询活跃预约（未取消的门诊预约）
        List<WkfAppointmentQueue> activeAppointments = appointmentQueueService.list(
                new LambdaQueryWrapper<WkfAppointmentQueue>()
                        .in(WkfAppointmentQueue::getSlotId, slotIds)
                        .eq(WkfAppointmentQueue::getAppointmentKind, "OUTPATIENT_BOOKING")
                        .isNull(WkfAppointmentQueue::getCancellationAt)
                        .eq(WkfAppointmentQueue::getIsDeleted, false)
        );

        // 5. 根据处置建议执行预约处理
        if (dto.getAppointmentDisposals() != null) {
            for (AppointmentDisposalDTO disposal : dto.getAppointmentDisposals()) {
                processAppointmentDisposal(disposal);
            }
        }

        // 6. 停用日程：status_enum = 'inactive'
        WkfSchedule updateSchedule = new WkfSchedule();
        updateSchedule.setId(scheduleIdStr);
        updateSchedule.setStatusEnum("inactive");
        scheduleService.updateById(updateSchedule);

        // 7. 释放预览冻结（软删除冻结记录）
        slotFreezeService.update(
                new LambdaUpdateWrapper<BkgStopClinicSlotFreeze>()
                        .set(BkgStopClinicSlotFreeze::getIsDeleted, true)
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, scheduleIdStr)
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );

        //还未实现发送停诊通知功能
        if (Boolean.TRUE.equals(dto.getSendNotification())) {
            log.info("发送停诊通知，scheduleId: {}", scheduleIdStr);
        }

        log.info("整日停诊执行成功，scheduleId: {}", scheduleIdStr);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void executePartialStopClinic(PartialStopClinicExecuteDTO dto) {
        log.info("执行部分停诊，scheduleId: {}, stopStartTime: {}, stopEndTime: {}",
                dto.getScheduleId(), dto.getStopStartTime(), dto.getStopEndTime());

        // 1. 查询日程
        String scheduleIdStr = String.valueOf(dto.getScheduleId());
        WkfSchedule schedule = scheduleService.getById(scheduleIdStr);
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 查询该日程下的所有号源
        List<WkfSlot> allSlots = slotService.list(
                new LambdaQueryWrapper<WkfSlot>()
                        .eq(WkfSlot::getScheduleId, scheduleIdStr)
                        .eq(WkfSlot::getIsDeleted, false)
        );

        if (allSlots.isEmpty()) {
            throw new BusinessException("该日程下没有号源");
        }

        // 3. 筛选出停诊时间段内的号源
        List<WkfSlot> affectedSlots = allSlots.stream()
                .filter(slot -> isSlotInStopPeriod(slot, dto.getStopStartTime(), dto.getStopEndTime()))
                .collect(Collectors.toList());

        if (affectedSlots.isEmpty()) {
            throw new BusinessException("停诊时间段内没有号源");
        }

        // 4. 获取受影响号源的 slotId
        List<String> affectedSlotIds = affectedSlots.stream()
                .map(WkfSlot::getId)
                .collect(Collectors.toList());

        // 5. 查询活跃预约
        List<WkfAppointmentQueue> activeAppointments = appointmentQueueService.list(
                new LambdaQueryWrapper<WkfAppointmentQueue>()
                        .in(WkfAppointmentQueue::getSlotId, affectedSlotIds)
                        .eq(WkfAppointmentQueue::getAppointmentKind, "OUTPATIENT_BOOKING")
                        .isNull(WkfAppointmentQueue::getCancellationAt)
                        .eq(WkfAppointmentQueue::getIsDeleted, false)
        );

        // 6. 根据处置建议执行预约处理
        if (dto.getAppointmentDisposals() != null) {
            for (AppointmentDisposalDTO disposal : dto.getAppointmentDisposals()) {
                processAppointmentDisposal(disposal);
            }
        }

        // 7. 将受影响的号源状态改为 busy-unavailable
        for (WkfSlot slot : affectedSlots) {
            WkfSlot updateSlot = new WkfSlot();
            updateSlot.setId(slot.getId());
            updateSlot.setStatusCode("busy-unavailable");
            slotService.updateById(updateSlot);
        }

        // 8. 释放预览冻结
        slotFreezeService.update(
                new LambdaUpdateWrapper<BkgStopClinicSlotFreeze>()
                        .set(BkgStopClinicSlotFreeze::getIsDeleted, true)
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, scheduleIdStr)
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );

        // 9. TODO: 还未实现
        if (Boolean.TRUE.equals(dto.getSendNotification())) {
            log.info("发送部分停诊通知，scheduleId: {}", scheduleIdStr);
        }

        log.info("部分停诊执行成功，scheduleId: {}, 受影响号源数: {}", scheduleIdStr, affectedSlots.size());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void unlockStopClinic(Long scheduleId) {
        log.info("解锁停诊，scheduleId: {}", scheduleId);

        // 1. 查询日程
        String scheduleIdStr = String.valueOf(scheduleId);
        WkfSchedule schedule = scheduleService.getById(scheduleIdStr);
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 释放冻结：软删除冻结记录
        boolean updated = slotFreezeService.update(
                new LambdaUpdateWrapper<BkgStopClinicSlotFreeze>()
                        .set(BkgStopClinicSlotFreeze::getIsDeleted, true)
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, scheduleIdStr)
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );

        if (!updated) {
            log.warn("没有找到需要释放的冻结记录，scheduleId: {}", scheduleIdStr);
        }

        log.info("停诊解锁成功，scheduleId: {}, 释放冻结记录: {}", scheduleIdStr, updated);
    }

    /**
     * 处理单个预约处置
     */
    private void processAppointmentDisposal(AppointmentDisposalDTO disposal) {
        String disposalType = disposal.getDisposalType();
        Long appointmentId = disposal.getAppointmentId();

        WkfAppointmentQueue appointment = appointmentQueueService.getById(String.valueOf(appointmentId));
        if (appointment == null) {
            log.warn("预约不存在，appointmentId: {}", appointmentId);
            return;
        }

        switch (disposalType) {
            case "TRANSFER":
                // 转交：迁移预约到目标号源
                WkfAppointmentQueue updateTransfer = new WkfAppointmentQueue();
                updateTransfer.setId(String.valueOf(appointmentId));
                updateTransfer.setSlotId(String.valueOf(disposal.getTargetSlotId()));
                appointmentQueueService.updateById(updateTransfer);
                log.info("预约转交成功，appointmentId: {}, targetSlotId: {}", appointmentId, disposal.getTargetSlotId());
                break;

            case "CANCEL":
                // 取消：更新取消信息
                WkfAppointmentQueue updateCancel = new WkfAppointmentQueue();
                updateCancel.setId(String.valueOf(appointmentId));
                updateCancel.setCancellationReasonCode(disposal.getCancelReasonCode());
                updateCancel.setCancelReason(disposal.getCancelReason());
                updateCancel.setCancellationAt(LocalDateTime.now());
                appointmentQueueService.updateById(updateCancel);
                log.info("预约取消成功，appointmentId: {}", appointmentId);
                break;

            case "RESCHEDULE":
                // 改期：标记为待改期
                log.info("预约标记为待改期，appointmentId: {}", appointmentId);
                break;

            case "MANUAL":
                // 人工处理：不修改数据，仅记录日志
                log.info("预约标记为待人工处理，appointmentId: {}", appointmentId);
                break;

            default:
                log.warn("未知的处置类型: {}, appointmentId: {}", disposalType, appointmentId);
        }
    }

    /**
     * 判断号源是否在停诊时间段内
     */
    private boolean isSlotInStopPeriod(WkfSlot slot, String stopStartTime, String stopEndTime) {
        if (slot.getStartTime() == null) {
            return false;
        }

        // 简化处理：比较时间部分
        String slotStartTime = slot.getStartTime().toLocalTime().toString();
        return slotStartTime.compareTo(stopStartTime) >= 0
                && slotStartTime.compareTo(stopEndTime) < 0;
    }

    /**
     * 查询原医师 ID
     * @param scheduleId 日程 ID
     * @return 原医师 ID，如果未找到返回 null
     */
    private String getOriginalPractitionerId(String scheduleId) {
        // actorType = 1 表示医师（具体值需要根据项目规范确认）
        // 这里查询 schedule_actor 表中 actorType 为医师类型的参与者
        WkfScheduleActor actor = scheduleActorService.getOne(
                new LambdaQueryWrapper<WkfScheduleActor>()
                        .eq(WkfScheduleActor::getScheduleId, scheduleId)
                        .eq(WkfScheduleActor::getActorType, 1)  // 1 表示医师类型，需要根据实际业务确认
                        .eq(WkfScheduleActor::getIsDeleted, false)
                        .orderByAsc(WkfScheduleActor::getSortNo)
                        .last("LIMIT 1")
        );
        return actor != null ? actor.getActorId() : null;
    }
}