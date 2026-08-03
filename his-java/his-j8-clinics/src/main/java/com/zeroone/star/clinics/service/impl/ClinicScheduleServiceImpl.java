// service/impl/ClinicScheduleServiceImpl.java
package com.zeroone.star.clinics.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.clinics.entity.*;
import com.zeroone.star.clinics.exception.BusinessException;
import com.zeroone.star.clinics.service.*;
import com.zeroone.star.clinics.mapper.AdmPatientMapper;
import com.zeroone.star.project.components.sms.aliyun.SmsComponent;
import com.zeroone.star.project.components.sms.aliyun.SmsResult;
import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;
import com.zeroone.star.project.dto.j8.clinics.UnlockStopClinicDTO;
import com.zeroone.star.project.dto.j8.clinics.TransferItemDTO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicResultVO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicItemVO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicSuccessItemVO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicFailedItemVO;
import com.zeroone.star.project.vo.j8.clinics.SubstituteVO;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * 门诊排班管理业务服务实现
 */
@Slf4j
@Service
@RequiredArgsConstructor
public class ClinicScheduleServiceImpl implements ClinicScheduleService {

    /**
     * 短信签名名称（后续可从 Nacos 配置中心读取）
     */
    private static final String SMS_SIGN_NAME = "医院名称";


    // 注入 MyBatis-Plus 生成的基础 Service
    private final IWkfScheduleService scheduleService;
    private final IWkfSlotService slotService;
    private final IWkfAppointmentQueueService appointmentQueueService;
    private final IBkgStopClinicSlotFreezeService slotFreezeService;
    private final IBkgScheduleChangePolicyService changePolicyService;
    private final IWkfScheduleActorService scheduleActorService;
    private final AdmPatientMapper patientMapper;
    private final SmsComponent smsComponent;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SubstituteVO setSubstitute(SubstituteDTO dto) {
        log.info("设置替诊，scheduleId: {}, substitutePractitionerId: {}", dto.getScheduleId(), dto.getSubstitutePractitionerId());

        // 1. 查询日程
        WkfSchedule schedule = scheduleService.getById(dto.getScheduleId());
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 判断日程状态是否为 active
        if (!"active".equals(schedule.getStatusEnum())) {
            throw new BusinessException("仅已发布日程可替诊");
        }

        // 3. 查询原医师ID
        String originalPractitionerId = getOriginalPractitionerId(dto.getScheduleId());
        
        // 4. 判断替诊医师 ≠ 原医师
        if (originalPractitionerId != null && originalPractitionerId.equals(dto.getSubstitutePractitionerId())) {
            throw new BusinessException("替诊医师不能与原医师相同");
        }

        // 5. 执行替诊：更新 wkf_schedule 表
        WkfSchedule updateEntity = new WkfSchedule();
        updateEntity.setId(dto.getScheduleId());
        updateEntity.setSubstitutePractitionerId(dto.getSubstitutePractitionerId());
        updateEntity.setSubstituteReason(dto.getReason());
        scheduleService.updateById(updateEntity);

        // 6. 构建返回对象
        SubstituteVO vo = new SubstituteVO();
        vo.setScheduleId(dto.getScheduleId());
        vo.setOriginalPractitionerId(originalPractitionerId);
        vo.setSubstitutePractitionerId(dto.getSubstitutePractitionerId());
        vo.setSubstituteReason(dto.getReason());
        vo.setCleared(false);

        log.info("替诊设置成功，scheduleId: {}", dto.getScheduleId());
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SubstituteVO cancelSubstitute(String scheduleId) {
        log.info("撤销替诊，scheduleId: {}", scheduleId);

        // 1. 查询日程
        WkfSchedule schedule = scheduleService.getById(scheduleId);
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 判断是否已替诊
        if (schedule.getSubstitutePractitionerId() == null || schedule.getSubstitutePractitionerId().isEmpty()) {
            throw new BusinessException("该日程未设置替诊");
        }

        // 3. 保存替诊信息用于返回
        String originalPractitionerId = getOriginalPractitionerId(scheduleId);
        String substitutePractitionerId = schedule.getSubstitutePractitionerId();
        String substituteReason = schedule.getSubstituteReason();

        // 4. 执行撤销：清空替诊信息
        WkfSchedule updateEntity = new WkfSchedule();
        updateEntity.setId(scheduleId);
        updateEntity.setSubstitutePractitionerId(null);
        updateEntity.setSubstituteReason(null);
        scheduleService.updateById(updateEntity);

        // 5. 构建返回对象
        SubstituteVO vo = new SubstituteVO();
        vo.setScheduleId(scheduleId);
        vo.setOriginalPractitionerId(originalPractitionerId);
        vo.setSubstitutePractitionerId(substitutePractitionerId);
        vo.setSubstituteReason(substituteReason);
        vo.setCleared(true);

        log.info("替诊撤销成功，scheduleId: {}", scheduleId);
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StopClinicResultVO executeAllDayStopClinic(AllDayStopClinicExecuteDTO dto) {
        log.info("执行整日停诊，scheduleId: {}", dto.getScheduleId());

        // 1. 查询日程
        WkfSchedule schedule = scheduleService.getById(dto.getScheduleId());
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 查询该日程下的所有号源
        List<WkfSlot> slots = slotService.list(
                new LambdaQueryWrapper<WkfSlot>()
                        .eq(WkfSlot::getScheduleId, dto.getScheduleId())
                        .eq(WkfSlot::getIsDeleted, false)
        );

        if (slots.isEmpty()) {
            throw new BusinessException("该日程下没有号源");
        }

        // 3. 查询所有预约
        List<String> slotIds = slots.stream()
                .map(WkfSlot::getId)
                .collect(Collectors.toList());
        
        List<WkfAppointmentQueue> appointments = appointmentQueueService.list(
                new LambdaQueryWrapper<WkfAppointmentQueue>()
                        .in(WkfAppointmentQueue::getSlotId, slotIds)
                        .eq(WkfAppointmentQueue::getAppointmentKind, "OUTPATIENT_BOOKING")
                        .eq(WkfAppointmentQueue::getIsDeleted, false)
        );

        // 4. 处理每个预约（整日停诊默认取消所有预约）
        List<StopClinicItemVO> items = new ArrayList<>();
        List<StopClinicSuccessItemVO> successItems = new ArrayList<>();
        List<StopClinicFailedItemVO> failedItems = new ArrayList<>();

        for (WkfAppointmentQueue appointment : appointments) {
            StopClinicItemVO item = processAppointmentWithResult(
                    appointment, "CANCEL", null, new HashMap<>());
            items.add(item);

            if ("SUCCESS".equals(item.getDispositionStatus())) {
                successItems.add(buildSuccessItem(item));
            } else {
                failedItems.add(buildFailedItem(item));
            }
        }

        // 5. 停用日程：status_enum = 'inactive'
        WkfSchedule updateSchedule = new WkfSchedule();
        updateSchedule.setId(dto.getScheduleId());
        updateSchedule.setStatusEnum("inactive");
        scheduleService.updateById(updateSchedule);

        // 6. 将所有号源状态改为 busy-unavailable
        for (WkfSlot slot : slots) {
            WkfSlot updateSlot = new WkfSlot();
            updateSlot.setId(slot.getId());
            updateSlot.setStatusCode("busy-unavailable");
            slotService.updateById(updateSlot);
        }

        // 7. 释放预览冻结（软删除冻结记录）
        slotFreezeService.update(
                new LambdaUpdateWrapper<BkgStopClinicSlotFreeze>()
                        .set(BkgStopClinicSlotFreeze::getIsDeleted, true)
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, dto.getScheduleId())
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );

        // 8. 构建返回对象
        StopClinicResultVO vo = buildStopClinicResultVO(
                dto.getScheduleId(), schedule, slots, appointments, items, successItems, failedItems);

        // 9. 发送短信通知（异步）
        if (Boolean.TRUE.equals(dto.getSendNotification())) {
            BkgScheduleChangePolicy policy = loadPolicy(schedule.getOrganizationId());
            sendStopClinicNotification(appointments, policy, dto.getReason(), "CANCEL");
        }

        log.info("整日停诊执行成功，scheduleId: {}, 预约数: {}, 成功: {}, 失败: {}", 
                dto.getScheduleId(), appointments.size(), successItems.size(), failedItems.size());
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StopClinicResultVO executePartialStopClinic(PartialStopClinicExecuteDTO dto) {
        log.info("执行部分停诊，scheduleId: {}, slotIds: {}, dispositionMode: {}", 
                dto.getScheduleId(), dto.getSlotIds(), dto.getDispositionMode());

        // 1. 校验参数
        if (dto.getSlotIds() == null || dto.getSlotIds().isEmpty()) {
            throw new BusinessException("号源ID列表不能为空");
        }

        // 2. 查询日程
        WkfSchedule schedule = scheduleService.getById(dto.getScheduleId());
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 3. 根据 slotIds 查询号源
        List<WkfSlot> affectedSlots = slotService.list(
                new LambdaQueryWrapper<WkfSlot>()
                        .in(WkfSlot::getId, dto.getSlotIds())
                        .eq(WkfSlot::getScheduleId, dto.getScheduleId())
                        .eq(WkfSlot::getIsDeleted, false)
        );

        if (affectedSlots.isEmpty()) {
            throw new BusinessException("指定的号源不存在");
        }

        // 4. 校验传入的 slotIds 是否都有效
        if (affectedSlots.size() != dto.getSlotIds().size()) {
            log.warn("部分号源ID无效，传入: {}, 有效: {}", dto.getSlotIds().size(), affectedSlots.size());
        }

        // 5. 查询这些号源下的预约
        List<String> slotIds = affectedSlots.stream()
                .map(WkfSlot::getId)
                .collect(Collectors.toList());
        
        List<WkfAppointmentQueue> appointments = appointmentQueueService.list(
                new LambdaQueryWrapper<WkfAppointmentQueue>()
                        .in(WkfAppointmentQueue::getSlotId, slotIds)
                        .eq(WkfAppointmentQueue::getAppointmentKind, "OUTPATIENT_BOOKING")
                        .eq(WkfAppointmentQueue::getIsDeleted, false)
        );

        // 6. 构建转交映射（appointmentId -> targetSlotId）
        Map<String, String> transferMap = buildTransferMap(dto.getTransferItems());

        // 7. 处理每个预约
        List<StopClinicItemVO> items = new ArrayList<>();
        List<StopClinicSuccessItemVO> successItems = new ArrayList<>();
        List<StopClinicFailedItemVO> failedItems = new ArrayList<>();

        for (WkfAppointmentQueue appointment : appointments) {
            StopClinicItemVO item = processAppointmentWithResult(
                    appointment, dto.getDispositionMode(), dto.getTargetPractitionerId(), transferMap);
            items.add(item);

            if ("SUCCESS".equals(item.getDispositionStatus())) {
                successItems.add(buildSuccessItem(item));
            } else {
                failedItems.add(buildFailedItem(item));
            }
        }

        // 8. 将受影响的号源状态改为 busy-unavailable
        for (WkfSlot slot : affectedSlots) {
            WkfSlot updateSlot = new WkfSlot();
            updateSlot.setId(slot.getId());
            updateSlot.setStatusCode("busy-unavailable");
            slotService.updateById(updateSlot);
        }

        // 9. 释放预览冻结
        slotFreezeService.update(
                new LambdaUpdateWrapper<BkgStopClinicSlotFreeze>()
                        .set(BkgStopClinicSlotFreeze::getIsDeleted, true)
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, dto.getScheduleId())
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );

        // 10. 构建返回对象（包含真实统计数据）
        StopClinicResultVO vo = buildStopClinicResultVO(
                dto.getScheduleId(), schedule, affectedSlots, appointments, items, successItems, failedItems);

        // 11. 发送短信通知（异步）
        if (Boolean.TRUE.equals(dto.getSendNotification())) {
            BkgScheduleChangePolicy policy = loadPolicy(schedule.getOrganizationId());
            sendStopClinicNotification(appointments, policy, dto.getReason(), dto.getDispositionMode());
        }

        log.info("部分停诊执行成功，scheduleId: {}, 预约数: {}, 成功: {}, 失败: {}", 
                dto.getScheduleId(), appointments.size(), successItems.size(), failedItems.size());
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void unlockStopClinic(UnlockStopClinicDTO dto) {
        log.info("解锁停诊，scheduleId: {}, organizationId: {}, slotIds: {}", 
                dto.getScheduleId(), dto.getOrganizationId(), dto.getSlotIds());

        // 1. 查询日程
        WkfSchedule schedule = scheduleService.getById(dto.getScheduleId());
        if (schedule == null || Boolean.TRUE.equals(schedule.getIsDeleted())) {
            throw new BusinessException("日程不存在");
        }

        // 2. 释放冻结：软删除冻结记录
        boolean updated = slotFreezeService.update(
                new LambdaUpdateWrapper<BkgStopClinicSlotFreeze>()
                        .set(BkgStopClinicSlotFreeze::getIsDeleted, true)
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, dto.getScheduleId())
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );

        if (!updated) {
            log.warn("没有找到需要释放的冻结记录，scheduleId: {}", dto.getScheduleId());
        }

        log.info("停诊解锁成功，scheduleId: {}, 释放冻结记录: {}", dto.getScheduleId(), updated);
    }

    /**
     * 构建停诊结果返回对象（完整版）
     */
    private StopClinicResultVO buildStopClinicResultVO(
            String scheduleId, 
            WkfSchedule schedule, 
            List<WkfSlot> slots,
            List<WkfAppointmentQueue> appointments,
            List<StopClinicItemVO> items,
            List<StopClinicSuccessItemVO> successItems,
            List<StopClinicFailedItemVO> failedItems) {
        
        // 1. 统计数据
        int appointmentCount = appointments != null ? appointments.size() : 0;
        int registeredCount = 0;
        int checkedInCount = 0;
        
        if (appointments != null) {
            // 已挂号：有 startInstant 且未取消
            registeredCount = (int) appointments.stream()
                    .filter(a -> a.getStartInstant() != null && a.getCancellationAt() == null)
                    .count();
            // 已签到：有 triageTime 且未取消
            checkedInCount = (int) appointments.stream()
                    .filter(a -> a.getTriageTime() != null && a.getCancellationAt() == null)
                    .count();
        }

        // 2. 查询冻结记录
        List<BkgStopClinicSlotFreeze> freezeRecords = slotFreezeService.list(
                new LambdaQueryWrapper<BkgStopClinicSlotFreeze>()
                        .eq(BkgStopClinicSlotFreeze::getScheduleId, scheduleId)
                        .eq(BkgStopClinicSlotFreeze::getIsDeleted, false)
        );
        boolean sourceSlotsFrozen = freezeRecords != null && !freezeRecords.isEmpty();
        int frozenSlotCount = sourceSlotsFrozen ? freezeRecords.size() : 0;

        // 3. 判断是否有未处理的已签到患者
        boolean hasUnhandledCheckedIn = checkedInCount > 0 && failedItems != null && !failedItems.isEmpty();

        // 4. 从配置表读取默认处置策略
        BkgScheduleChangePolicy policy = loadPolicy(schedule.getOrganizationId());
        String defaultDispositionMode = policy != null && policy.getOnPractitionerStopMode() != null 
                ? policy.getOnPractitionerStopMode() 
                : "CANCEL"; // 默认取消
        String transferMatchRule = policy != null && policy.getTransferMatchRule() != null 
                ? policy.getTransferMatchRule() 
                : "PRACTITIONER_DEPT"; // 默认按科室匹配
        boolean blockDeactivateIfUnhandled = policy != null && policy.getBlockDeactivateIfUnhandled() != null 
                ? policy.getBlockDeactivateIfUnhandled() 
                : hasUnhandledCheckedIn;

        // 5. 构建返回对象
        StopClinicResultVO vo = new StopClinicResultVO();
        vo.setScheduleId(scheduleId);
        vo.setScheduleCount(1);
        vo.setSlotCount(slots.size());
        vo.setAppointmentCount(appointmentCount);
        vo.setRegisteredCount(registeredCount);
        vo.setCheckedInCount(checkedInCount);
        vo.setChannelCount(0); // 渠道统计暂未实现
        vo.setBlockDeactivateIfUnhandled(blockDeactivateIfUnhandled);
        vo.setDefaultDispositionMode(defaultDispositionMode);
        vo.setTransferMatchRule(transferMatchRule);
        vo.setSourceSlotsFrozen(sourceSlotsFrozen);
        vo.setFrozenSlotCount(frozenSlotCount);
        vo.setFreezeExpiresAt(freezeRecords != null && !freezeRecords.isEmpty() 
                ? String.valueOf(freezeRecords.get(0).getExpiresAt()) 
                : null);
        vo.setItems(items != null ? items : new ArrayList<>());
        vo.setSuccessCount(successItems != null ? successItems.size() : 0);
        vo.setFailedCount(failedItems != null ? failedItems.size() : 0);
        vo.setScheduleDeactivated("inactive".equals(schedule.getStatusEnum()));
        vo.setBlockedSlotCount(slots.size());
        vo.setSuccessItems(successItems != null ? successItems : new ArrayList<>());
        vo.setFailedItems(failedItems != null ? failedItems : new ArrayList<>());
        
        return vo;
    }

    /**
     * 加载停诊策略配置
     * 优先查询机构级配置，若无则查询租户级配置
     */
    private BkgScheduleChangePolicy loadPolicy(String organizationId) {
        // 1. 先查机构级配置
        BkgScheduleChangePolicy policy = changePolicyService.getOne(
                new LambdaQueryWrapper<BkgScheduleChangePolicy>()
                        .eq(BkgScheduleChangePolicy::getOrganizationId, organizationId)
                        .eq(BkgScheduleChangePolicy::getIsDeleted, false)
                        .orderByDesc(BkgScheduleChangePolicy::getUpdatedAt)
                        .last("LIMIT 1")
        );
        
        // 2. 若无机构级配置，查租户级配置（organizationId 为空）
        if (policy == null) {
            policy = changePolicyService.getOne(
                    new LambdaQueryWrapper<BkgScheduleChangePolicy>()
                            .isNull(BkgScheduleChangePolicy::getOrganizationId)
                            .eq(BkgScheduleChangePolicy::getIsDeleted, false)
                            .orderByDesc(BkgScheduleChangePolicy::getUpdatedAt)
                            .last("LIMIT 1")
            );
        }
        
        return policy;
    }

    /**
     * 构建转交映射（appointmentId -> targetSlotId）
     */
    private Map<String, String> buildTransferMap(List<TransferItemDTO> transferItems) {
        Map<String, String> transferMap = new HashMap<>();
        if (transferItems != null) {
            for (TransferItemDTO item : transferItems) {
                if (item.getAppointmentId() != null && item.getTargetSlotId() != null) {
                    transferMap.put(item.getAppointmentId(), item.getTargetSlotId());
                }
            }
        }
        return transferMap;
    }

    /**
     * 处理单个预约并返回结果
     */
    private StopClinicItemVO processAppointmentWithResult(
            WkfAppointmentQueue appointment,
            String dispositionMode,
            String targetPractitionerId,
            Map<String, String> transferMap) {
        
        StopClinicItemVO item = new StopClinicItemVO();
        item.setAppointmentId(appointment.getId());
        item.setBusNo(appointment.getBusNo());
        item.setPatientId(appointment.getPatientId());
        item.setPatientName(getPatientName(appointment.getPatientId()));
        item.setSourceSlotId(appointment.getSlotId());
        
        // 设置源号源时间
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        if (appointment.getStartInstant() != null) {
            item.setSourceStartTime(appointment.getStartInstant().format(formatter));
        }
        if (appointment.getEndInstant() != null) {
            item.setSourceEndTime(appointment.getEndInstant().format(formatter));
        }

        try {
            // 根据处置模式处理
            switch (dispositionMode != null ? dispositionMode : "CANCEL") {
                case "TRANSFER":
                    processTransfer(appointment, transferMap, targetPractitionerId, item);
                    break;
                case "CANCEL":
                    processCancel(appointment, item);
                    break;
                case "RESCHEDULE":
                    processReschedule(appointment, transferMap, item);
                    break;
                default:
                    item.setDispositionStatus("FAILED");
                    item.setDispositionMessage("不支持的处置模式: " + dispositionMode);
            }
        } catch (Exception e) {
            log.error("处理预约失败，appointmentId: {}, error: {}", appointment.getId(), e.getMessage(), e);
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("处理失败: " + e.getMessage());
        }

        return item;
    }

    /**
     * 处理转交
     */
    private void processTransfer(
            WkfAppointmentQueue appointment,
            Map<String, String> transferMap,
            String targetPractitionerId,
            StopClinicItemVO item) {
        
        String targetSlotId = transferMap.get(appointment.getId());
        
        if (targetSlotId == null || targetSlotId.isEmpty()) {
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("未指定目标号源");
            return;
        }

        // 查询目标号源
        WkfSlot targetSlot = slotService.getById(targetSlotId);
        if (targetSlot == null || Boolean.TRUE.equals(targetSlot.getIsDeleted())) {
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("目标号源不存在");
            return;
        }

        if ("busy-unavailable".equals(targetSlot.getStatusCode())) {
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("目标号源不可用");
            return;
        }

        // 更新预约到目标号源
        WkfAppointmentQueue updateAppointment = new WkfAppointmentQueue();
        updateAppointment.setId(appointment.getId());
        updateAppointment.setSlotId(targetSlotId);
        appointmentQueueService.updateById(updateAppointment);

        // 查询源号源获取 bookedCount
        WkfSlot sourceSlot = slotService.getById(appointment.getSlotId());
        int sourceBookedCount = sourceSlot != null && sourceSlot.getBookedCount() != null ? sourceSlot.getBookedCount() : 0;

        // 减少源号源的预约数
        WkfSlot updateSourceSlot = new WkfSlot();
        updateSourceSlot.setId(appointment.getSlotId());
        updateSourceSlot.setBookedCount(Math.max(0, sourceBookedCount - 1));
        slotService.updateById(updateSourceSlot);

        // 增加目标号源的预约数
        WkfSlot updateTargetSlot = new WkfSlot();
        updateTargetSlot.setId(targetSlotId);
        updateTargetSlot.setBookedCount((targetSlot.getBookedCount() != null ? targetSlot.getBookedCount() : 0) + 1);
        slotService.updateById(updateTargetSlot);

        // 设置目标号源时间
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        item.setTargetSlotId(targetSlotId);
        if (targetSlot.getStartTime() != null) {
            item.setTargetStartTime(targetSlot.getStartTime().format(formatter));
        }
        if (targetSlot.getEndTime() != null) {
            item.setTargetEndTime(targetSlot.getEndTime().format(formatter));
        }
        item.setDispositionStatus("SUCCESS");
        item.setDispositionMessage("转交成功");
    }

    /**
     * 处理取消
     */
    private void processCancel(WkfAppointmentQueue appointment, StopClinicItemVO item) {
        // 检查是否已签到（已签到不能直接取消）
        if (appointment.getTriageTime() != null) {
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("患者已签到，不能直接取消，需现场处理");
            return;
        }

        // 软删除预约
        WkfAppointmentQueue updateAppointment = new WkfAppointmentQueue();
        updateAppointment.setId(appointment.getId());
        updateAppointment.setIsDeleted(true);
        updateAppointment.setCancellationAt(java.time.LocalDateTime.now());
        updateAppointment.setCancellationReasonCode("CLINIC_STOP");
        updateAppointment.setCancelReason("医师停诊");
        appointmentQueueService.updateById(updateAppointment);

        // 查询号源获取 bookedCount
        WkfSlot sourceSlot = slotService.getById(appointment.getSlotId());
        int sourceBookedCount = sourceSlot != null && sourceSlot.getBookedCount() != null ? sourceSlot.getBookedCount() : 0;

        // 减少号源预约数
        WkfSlot updateSlot = new WkfSlot();
        updateSlot.setId(appointment.getSlotId());
        updateSlot.setBookedCount(Math.max(0, sourceBookedCount - 1));
        slotService.updateById(updateSlot);

        item.setDispositionStatus("SUCCESS");
        item.setDispositionMessage("取消成功");
    }

    /**
     * 处理改期
     */
    private void processReschedule(
            WkfAppointmentQueue appointment,
            Map<String, String> transferMap,
            StopClinicItemVO item) {
        
        // 改期逻辑类似转交，但需要特殊标记
        String targetSlotId = transferMap.get(appointment.getId());
        
        if (targetSlotId == null || targetSlotId.isEmpty()) {
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("未指定目标号源");
            return;
        }

        // 查询目标号源
        WkfSlot targetSlot = slotService.getById(targetSlotId);
        if (targetSlot == null || Boolean.TRUE.equals(targetSlot.getIsDeleted())) {
            item.setDispositionStatus("FAILED");
            item.setDispositionMessage("目标号源不存在");
            return;
        }

        // 更新预约
        WkfAppointmentQueue updateAppointment = new WkfAppointmentQueue();
        updateAppointment.setId(appointment.getId());
        updateAppointment.setSlotId(targetSlotId);
        updateAppointment.setCancellationReasonCode("RESCHEDULE");
        updateAppointment.setCancelReason("停诊改期");
        appointmentQueueService.updateById(updateAppointment);

        // 查询源号源获取 bookedCount
        WkfSlot sourceSlot = slotService.getById(appointment.getSlotId());
        int sourceBookedCount = sourceSlot != null && sourceSlot.getBookedCount() != null ? sourceSlot.getBookedCount() : 0;

        // 更新号源计数
        WkfSlot updateSourceSlot = new WkfSlot();
        updateSourceSlot.setId(appointment.getSlotId());
        updateSourceSlot.setBookedCount(Math.max(0, sourceBookedCount - 1));
        slotService.updateById(updateSourceSlot);

        WkfSlot updateTargetSlot = new WkfSlot();
        updateTargetSlot.setId(targetSlotId);
        updateTargetSlot.setBookedCount((targetSlot.getBookedCount() != null ? targetSlot.getBookedCount() : 0) + 1);
        slotService.updateById(updateTargetSlot);

        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        item.setTargetSlotId(targetSlotId);
        if (targetSlot.getStartTime() != null) {
            item.setTargetStartTime(targetSlot.getStartTime().format(formatter));
        }
        if (targetSlot.getEndTime() != null) {
            item.setTargetEndTime(targetSlot.getEndTime().format(formatter));
        }
        item.setDispositionStatus("SUCCESS");
        item.setDispositionMessage("改期成功");
    }

    /**
     * 构建成功项目
     */
    private StopClinicSuccessItemVO buildSuccessItem(StopClinicItemVO item) {
        StopClinicSuccessItemVO vo = new StopClinicSuccessItemVO();
        vo.setAppointmentId(item.getAppointmentId());
        vo.setTargetSlotId(item.getTargetSlotId());
        vo.setMessage(item.getDispositionMessage());
        return vo;
    }

    /**
     * 构建失败项目
     */
    private StopClinicFailedItemVO buildFailedItem(StopClinicItemVO item) {
        StopClinicFailedItemVO vo = new StopClinicFailedItemVO();
        vo.setAppointmentId(item.getAppointmentId());
        vo.setTargetSlotId(item.getTargetSlotId());
        vo.setMessage(item.getDispositionMessage());
        return vo;
    }

    /**
     * 查询原医师 ID
     * @param scheduleId 日程 ID
     * @return 原医师 ID，如果未找到返回 null
     */
    private String getOriginalPractitionerId(String scheduleId) {
        WkfScheduleActor actor = scheduleActorService.getOne(
                new LambdaQueryWrapper<WkfScheduleActor>()
                        .eq(WkfScheduleActor::getScheduleId, scheduleId)
                        .eq(WkfScheduleActor::getActorType, 1)
                        .eq(WkfScheduleActor::getIsDeleted, false)
                        .orderByAsc(WkfScheduleActor::getSortNo)
                        .last("LIMIT 1")
        );
        return actor != null ? actor.getActorId() : null;
    }

    /**
     * 查询患者姓名
     * @param patientId 患者 ID
     * @return 患者姓名，如果未找到返回 null
     */
    private String getPatientName(String patientId) {
        if (patientId == null) {
            return null;
        }
        AdmPatient patient = patientMapper.selectById(patientId);
        return patient != null ? patient.getName() : null;
    }

    /**
     * 发送停诊通知短信（异步）
     * @param appointments 受影响的预约列表
     * @param policy 停诊策略配置
     * @param reason 停诊原因
     * @param dispositionMode 处置模式
     */
    private void sendStopClinicNotification(
            List<WkfAppointmentQueue> appointments,
            BkgScheduleChangePolicy policy,
            String reason,
            String dispositionMode) {
        
        if (appointments == null || appointments.isEmpty()) {
            return;
        }

        // 根据处置模式判断是否需要发送短信
        String templateCode = null;
        
        if ("CANCEL".equals(dispositionMode) || "RESCHEDULE".equals(dispositionMode)) {
            // 取消或改期：检查 notifyOnCancelStop 配置
            if (policy != null && Boolean.TRUE.equals(policy.getNotifyOnCancelStop())) {
                templateCode = "SMS_STOP_CLINIC_CANCEL"; // 停诊取消模板
            }
        } else if ("TRANSFER".equals(dispositionMode)) {
            // 转交：检查 notifyOnTransfer 配置
            if (policy != null && Boolean.TRUE.equals(policy.getNotifyOnTransfer())) {
                templateCode = "SMS_STOP_CLINIC_TRANSFER"; // 停诊转交模板
            }
        }

        if (templateCode == null) {
            log.info("停诊短信通知未启用，dispositionMode: {}", dispositionMode);
            return;
        }

        // 异步发送短信（不阻塞主流程）
        final String finalTemplateCode = templateCode;
        final String finalReason = reason;
        final String finalDispositionMode = dispositionMode;
        java.util.concurrent.CompletableFuture.runAsync(() -> {
            try {
                sendSmsToPatients(appointments, finalTemplateCode, finalReason, finalDispositionMode);
            } catch (Exception e) {
                log.error("发送停诊通知短信失败", e);
            }
        });
    }

    /**
     * 批量发送短信给患者
     */
    private void sendSmsToPatients(
            List<WkfAppointmentQueue> appointments,
            String templateCode,
            String reason,
            String dispositionMode) {
        
        // 收集患者手机号
        List<String> phoneNumbers = new ArrayList<>();
        Map<String, String> phoneToNameMap = new HashMap<>();
        
        for (WkfAppointmentQueue appointment : appointments) {
            if (appointment.getPatientId() == null) {
                continue;
            }
            
            // 查询患者信息获取手机号
            AdmPatient patient = patientMapper.selectById(appointment.getPatientId());
            if (patient != null && patient.getPhone() != null && !patient.getPhone().isEmpty()) {
                String phone = patient.getPhone();
                if (!phoneNumbers.contains(phone)) {
                    phoneNumbers.add(phone);
                    phoneToNameMap.put(phone, patient.getName());
                }
            }
        }

        if (phoneNumbers.isEmpty()) {
            log.warn("没有找到有效的患者手机号，无法发送停诊通知");
            return;
        }

        log.info("开始发送停诊通知短信，接收人数: {}, 模板: {}", phoneNumbers.size(), templateCode);

        // 批量发送短信（每次最多100个）
        int batchSize = 100;
        for (int i = 0; i < phoneNumbers.size(); i += batchSize) {
            int end = Math.min(i + batchSize, phoneNumbers.size());
            List<String> batchPhones = phoneNumbers.subList(i, end);
            
            // 构建模板参数
            List<Map<String, String>> templateParams = batchPhones.stream()
                    .map(phone -> {
                        Map<String, String> params = new HashMap<>();
                        params.put("patientName", phoneToNameMap.getOrDefault(phone, "患者"));
                        params.put("reason", reason != null ? reason : "医师停诊");
                        params.put("action", getActionText(dispositionMode));
                        return params;
                    })
                    .collect(Collectors.toList());

            // 发送短信
            SmsResult result = smsComponent.sendSmsBatch(
                    batchPhones,
                    SMS_SIGN_NAME,
                    templateCode,
                    templateParams
            );

            if ("OK".equals(result.getCode())) {
                log.info("停诊通知短信发送成功，批次: {}-{}", i, end);
            } else {
                log.error("停诊通知短信发送失败，批次: {}-{}, 错误: {}", i, end, result.getMessage());
            }
        }
    }

    /**
     * 获取处置模式的中文描述
     */
    private String getActionText(String dispositionMode) {
        switch (dispositionMode != null ? dispositionMode : "CANCEL") {
            case "TRANSFER":
                return "已转交至其他医师";
            case "RESCHEDULE":
                return "已改期至其他时间";
            case "CANCEL":
            default:
                return "已取消预约";
        }
    }
}