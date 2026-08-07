package com.zeroone.star.ons.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.ons.entity.PatientConsumableItemDO;
import com.zeroone.star.ons.entity.WkfAppointmentQueue;
import com.zeroone.star.ons.entity.WkfServiceRequest;
import com.zeroone.star.ons.mapper.PatientConsumableItemMapper;
import com.zeroone.star.ons.mapper.WkfAppointmentQueueMapper;
import com.zeroone.star.ons.mapper.WkfServiceRequestMapper;
import com.zeroone.star.ons.service.IOnsService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.BatchExecuteResultVO;
import com.zeroone.star.project.vo.j5.ons.ExecutionRecordVO;
import com.zeroone.star.project.vo.j5.ons.PatientConsumableItemVO;
import com.zeroone.star.project.vo.j5.ons.PatientQueueVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.CollectionUtils;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

@Slf4j
@Service
public class OnsServiceImpl implements IOnsService {

    @Resource
    private WkfServiceRequestMapper wkfServiceRequestMapper;

    @Resource
    private WkfAppointmentQueueMapper wkfAppointmentQueueMapper;

    @Resource
    private PatientConsumableItemMapper patientConsumableItemMapper;

    private static final DateTimeFormatter DATE_TIME_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    private static final DateTimeFormatter DATE_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<BatchExecuteResultVO> execute(ExecuteRequest request) {
        if (request == null) {
            return JsonVO.fail("请求参数不能为空");
        }
        if (CollectionUtils.isEmpty(request.getRecord_ids())) {
            return JsonVO.fail("执行记录ID列表不能为空");
        }
        if (!StringUtils.hasText(request.getEncounter_id())) {
            return JsonVO.fail("就诊ID不能为空");
        }
        if (!StringUtils.hasText(request.getExecutor_id())) {
            return JsonVO.fail("执行者ID不能为空");
        }

        String idempotencyKey = request.getIdempotency_key();
        if (!StringUtils.hasText(idempotencyKey)) {
            idempotencyKey = UUID.randomUUID().toString();
        }

        LocalDateTime executeTime = StringUtils.hasText(request.getExecute_time())
                ? LocalDateTime.parse(request.getExecute_time(), DATE_TIME_FORMATTER)
                : LocalDateTime.now();

        BatchExecuteResultVO result = new BatchExecuteResultVO();
        result.setTotalCount(request.getRecord_ids().size());
        result.setSuccessIds(new ArrayList<>());
        result.setFailedItems(new ArrayList<>());
        result.setExecuteTime(executeTime.format(DATE_TIME_FORMATTER));
        result.setIdempotencyKey(idempotencyKey);

        int successCount = 0;
        int failedCount = 0;

        for (String recordId : request.getRecord_ids()) {
            try {
                WkfServiceRequest requestDO = wkfServiceRequestMapper.selectById(recordId);
                if (requestDO == null) {
                    addFailedItem(result, recordId, "NOT_FOUND", "记录不存在");
                    failedCount++;
                    continue;
                }

                if (!"ACTIVE".equals(requestDO.getStatusEnum())) {
                    addFailedItem(result, recordId, "STATUS_INVALID", "当前状态不允许执行");
                    failedCount++;
                    continue;
                }

                if (!request.getEncounter_id().equals(requestDO.getEncounterId())) {
                    addFailedItem(result, recordId, "ENCOUNTER_MISMATCH", "就诊ID不匹配");
                    failedCount++;
                    continue;
                }

                requestDO.setPerformFlag(1);
                requestDO.setStatusEnum("COMPLETED");
                requestDO.setPerformerId(request.getExecutor_id());
                requestDO.setPerformerTypeCode("NURSE");
                requestDO.setCheckTime(executeTime);
                requestDO.setOccurrenceStartTime(executeTime);
                requestDO.setRemark(request.getRemark());
                requestDO.setExecutionCount(requestDO.getExecutionCount() != null ? requestDO.getExecutionCount() + 1 : 1);

                if (wkfServiceRequestMapper.updateById(requestDO) > 0) {
                    result.getSuccessIds().add(recordId);
                    successCount++;
                } else {
                    addFailedItem(result, recordId, "UPDATE_FAILED", "更新失败");
                    failedCount++;
                }
            } catch (Exception e) {
                log.error("执行记录 {} 失败", recordId, e);
                addFailedItem(result, recordId, "SYSTEM_ERROR", "系统错误: " + e.getMessage());
                failedCount++;
            }
        }

        result.setSuccessCount(successCount);
        result.setFailedCount(failedCount);
        result.setHasPartialSuccess(successCount > 0 && failedCount > 0);

        if (successCount == 0) {
            return JsonVO.create(result, ResultStatus.FAIL.getCode(), "执行失败");
        }

        return JsonVO.success(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<BatchExecuteResultVO> cancelExecute(CancelExecuteRequest request) {
        if (request == null) {
            return JsonVO.fail("请求参数不能为空");
        }
        if (CollectionUtils.isEmpty(request.getRecord_ids())) {
            return JsonVO.fail("执行记录ID列表不能为空");
        }
        if (!StringUtils.hasText(request.getEncounter_id())) {
            return JsonVO.fail("就诊ID不能为空");
        }
        if (!StringUtils.hasText(request.getCancel_reason())) {
            return JsonVO.fail("取消原因不能为空");
        }

        String idempotencyKey = request.getIdempotency_key();
        if (!StringUtils.hasText(idempotencyKey)) {
            idempotencyKey = UUID.randomUUID().toString();
        }

        BatchExecuteResultVO result = new BatchExecuteResultVO();
        result.setTotalCount(request.getRecord_ids().size());
        result.setSuccessIds(new ArrayList<>());
        result.setFailedItems(new ArrayList<>());
        result.setExecuteTime(LocalDateTime.now().format(DATE_TIME_FORMATTER));
        result.setIdempotencyKey(idempotencyKey);

        int successCount = 0;
        int failedCount = 0;

        for (String recordId : request.getRecord_ids()) {
            try {
                WkfServiceRequest requestDO = wkfServiceRequestMapper.selectById(recordId);
                if (requestDO == null) {
                    addFailedItem(result, recordId, "NOT_FOUND", "记录不存在");
                    failedCount++;
                    continue;
                }

                if ("CANCELLED".equals(requestDO.getStatusEnum())) {
                    addFailedItem(result, recordId, "ALREADY_CANCELLED", "记录已取消");
                    failedCount++;
                    continue;
                }

                if (!request.getEncounter_id().equals(requestDO.getEncounterId())) {
                    addFailedItem(result, recordId, "ENCOUNTER_MISMATCH", "就诊ID不匹配");
                    failedCount++;
                    continue;
                }

                requestDO.setStatusEnum("CANCELLED");
                requestDO.setReasonText(request.getCancel_reason());
                requestDO.setRemark("取消原因: " + request.getCancel_reason());

                if (wkfServiceRequestMapper.updateById(requestDO) > 0) {
                    result.getSuccessIds().add(recordId);
                    successCount++;
                } else {
                    addFailedItem(result, recordId, "UPDATE_FAILED", "更新失败");
                    failedCount++;
                }
            } catch (Exception e) {
                log.error("取消记录 {} 失败", recordId, e);
                addFailedItem(result, recordId, "SYSTEM_ERROR", "系统错误: " + e.getMessage());
                failedCount++;
            }
        }

        result.setSuccessCount(successCount);
        result.setFailedCount(failedCount);
        result.setHasPartialSuccess(successCount > 0 && failedCount > 0);

        if (successCount == 0) {
            return JsonVO.create(result, ResultStatus.FAIL.getCode(), "取消失败");
        }

        return JsonVO.success(result);
    }

    @Override
    public JsonVO<String> print(PrintRequest request) {
        if (request == null) {
            return JsonVO.fail("请求参数不能为空");
        }
        if (CollectionUtils.isEmpty(request.getRecord_ids())) {
            return JsonVO.fail("执行记录ID列表不能为空");
        }

        List<WkfServiceRequest> records = new ArrayList<>();
        for (String recordId : request.getRecord_ids()) {
            WkfServiceRequest record = wkfServiceRequestMapper.selectById(recordId);
            if (record != null) {
                records.add(record);
            }
        }

        if (records.isEmpty()) {
            return JsonVO.fail("未找到有效的服务请求记录");
        }

        // 更新打印次数
        for (WkfServiceRequest record : records) {
            record.setPrintCount(record.getPrintCount() != null ? record.getPrintCount() + 1 : 1);
            wkfServiceRequestMapper.updateById(record);
        }

        String printContent = buildPrintContent(records, request.getPrint_type());
        return JsonVO.success(printContent);
    }

    private String buildPrintContent(List<WkfServiceRequest> records, String printType) {
        StringBuilder sb = new StringBuilder();
        sb.append("========================================\n");
        sb.append("         服务请求打印单\n");
        sb.append("========================================\n");

        String typeName = getPrintTypeName(printType);
        sb.append("打印类型：").append(typeName).append("\n");
        sb.append("打印时间：").append(LocalDateTime.now().format(DATE_TIME_FORMATTER)).append("\n");
        sb.append("----------------------------------------\n");

        for (int i = 0; i < records.size(); i++) {
            WkfServiceRequest record = records.get(i);
            sb.append("\n").append(i + 1).append(". 业务编号：").append(record.getBusNo()).append("\n");
            sb.append("   处方编号：").append(record.getPrescriptionNo()).append("\n");
            sb.append("   患者ID：").append(record.getPatientId()).append("\n");
            sb.append("   就诊ID：").append(record.getEncounterId()).append("\n");
            sb.append("   项目分类：").append(record.getCategoryEnum()).append("\n");
            sb.append("   数量：").append(record.getQuantity()).append(" ").append(record.getUnitCode()).append("\n");
            sb.append("   状态：").append(getStatusName(record.getStatusEnum())).append("\n");
            sb.append("   优先级：").append(getPriorityName(record.getPriorityEnum())).append("\n");
            sb.append("   开立时间：").append(record.getAuthoredTime() != null ?
                    record.getAuthoredTime().format(DATE_TIME_FORMATTER) : "未开立").append("\n");
            sb.append("   执行者：").append(record.getPerformerId() != null ? record.getPerformerId() : "未指定").append("\n");
            sb.append("   备注：").append(record.getRemark() != null ? record.getRemark() : "无").append("\n");
        }

        sb.append("\n========================================\n");
        return sb.toString();
    }

    @Override
    public JsonVO<PageDTO<ExecutionRecordVO>> getServiceRequests(ExecutionRecordQuery query) {
        if (query == null) {
            query = new ExecutionRecordQuery();
        }

        Page<WkfServiceRequest> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfServiceRequest> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfServiceRequest::getIsDeleted, 0);

        if (StringUtils.hasText(query.getPatient_id())) {
            wrapper.eq(WkfServiceRequest::getPatientId, query.getPatient_id());
        }
        if (StringUtils.hasText(query.getEncounter_id())) {
            wrapper.eq(WkfServiceRequest::getEncounterId, query.getEncounter_id());
        }
        if (StringUtils.hasText(query.getStatus_enum())) {
            wrapper.eq(WkfServiceRequest::getStatusEnum, query.getStatus_enum());
        }
        if (StringUtils.hasText(query.getCategory_enum())) {
            wrapper.eq(WkfServiceRequest::getCategoryEnum, query.getCategory_enum());
        }
        if (StringUtils.hasText(query.getPriority_enum())) {
            wrapper.eq(WkfServiceRequest::getPriorityEnum, query.getPriority_enum());
        }
        if (StringUtils.hasText(query.getPrescription_no())) {
            wrapper.like(WkfServiceRequest::getPrescriptionNo, query.getPrescription_no());
        }
        if (StringUtils.hasText(query.getStart_date())) {
            wrapper.ge(WkfServiceRequest::getAuthoredTime, query.getStart_date() + " 00:00:00");
        }
        if (StringUtils.hasText(query.getEnd_date())) {
            wrapper.le(WkfServiceRequest::getAuthoredTime, query.getEnd_date() + " 23:59:59");
        }

        wrapper.orderByDesc(WkfServiceRequest::getCreatedAt);

        IPage<WkfServiceRequest> resultPage = wkfServiceRequestMapper.selectPage(page, wrapper);

        List<ExecutionRecordVO> voList = resultPage.getRecords().stream()
                .map(this::convertToExecutionRecordVO)
                .collect(Collectors.toList());

        PageDTO<ExecutionRecordVO> pageDTO = new PageDTO<>();
        pageDTO.setRows(voList);
        pageDTO.setTotal(resultPage.getTotal());
        pageDTO.setPageIndex(resultPage.getCurrent());
        pageDTO.setPageSize(resultPage.getSize());
        pageDTO.setPages(resultPage.getPages());

        return JsonVO.success(pageDTO);
    }

    @Override
    public JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query) {
        if (query == null) {
            query = new PatientQueueQuery();
        }

        Page<WkfAppointmentQueue> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfAppointmentQueue> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfAppointmentQueue::getIsDeleted, 0);

        if (StringUtils.hasText(query.getOrganization_id())) {
            wrapper.eq(WkfAppointmentQueue::getOrganizationId, query.getOrganization_id());
        }
        if (StringUtils.hasText(query.getPatient_id())) {
            wrapper.eq(WkfAppointmentQueue::getPatientId, query.getPatient_id());
        }
        if (StringUtils.hasText(query.getEncounter_id())) {
            wrapper.eq(WkfAppointmentQueue::getEncounterId, query.getEncounter_id());
        }
        if (StringUtils.hasText(query.getAppointment_kind())) {
            wrapper.eq(WkfAppointmentQueue::getAppointmentKind, query.getAppointment_kind());
        }
        if (StringUtils.hasText(query.getService_type_code())) {
            wrapper.eq(WkfAppointmentQueue::getServiceTypeCode, query.getService_type_code());
        }
        if (StringUtils.hasText(query.getDestination_location_id())) {
            wrapper.eq(WkfAppointmentQueue::getDestinationLocationId, query.getDestination_location_id());
        }
        if (StringUtils.hasText(query.getTriage_level())) {
            wrapper.eq(WkfAppointmentQueue::getTriageLevel, query.getTriage_level());
        }
        if (StringUtils.hasText(query.getBooking_date())) {
            wrapper.ge(WkfAppointmentQueue::getStartInstant, query.getBooking_date() + " 00:00:00");
            wrapper.le(WkfAppointmentQueue::getStartInstant, query.getBooking_date() + " 23:59:59");
        }

        wrapper.orderByAsc(WkfAppointmentQueue::getDisplayOrder);
        wrapper.orderByDesc(WkfAppointmentQueue::getCreatedAt);

        IPage<WkfAppointmentQueue> resultPage = wkfAppointmentQueueMapper.selectPage(page, wrapper);

        List<PatientQueueVO> voList = resultPage.getRecords().stream()
                .map(this::convertToPatientQueueVO)
                .collect(Collectors.toList());

        PageDTO<PatientQueueVO> pageDTO = new PageDTO<>();
        pageDTO.setRows(voList);
        pageDTO.setTotal(resultPage.getTotal());
        pageDTO.setPageIndex(resultPage.getCurrent());
        pageDTO.setPageSize(resultPage.getSize());
        pageDTO.setPages(resultPage.getPages());

        return JsonVO.success(pageDTO);
    }

    @Override
    public JsonVO<List<ExecutionRecordVO>> getPatientServiceRequests(String patientId) {
        if (!StringUtils.hasText(patientId)) {
            return JsonVO.fail("患者ID不能为空");
        }

        LambdaQueryWrapper<WkfServiceRequest> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfServiceRequest::getIsDeleted, 0);
        wrapper.eq(WkfServiceRequest::getPatientId, patientId);
        wrapper.orderByDesc(WkfServiceRequest::getCreatedAt);

        List<WkfServiceRequest> items = wkfServiceRequestMapper.selectList(wrapper);
        List<ExecutionRecordVO> voList = items.stream()
                .map(this::convertToExecutionRecordVO)
                .collect(Collectors.toList());

        return JsonVO.success(voList);
    }

    @Override
    public JsonVO<List<PatientConsumableItemVO>> getPatientConsumableItems(String patientId) {
        if (!StringUtils.hasText(patientId)) {
            return JsonVO.fail("患者ID不能为空");
        }

        List<PatientConsumableItemDO> doList = patientConsumableItemMapper.selectByPatientId(patientId);
        List<PatientConsumableItemVO> voList = doList.stream()
                .map(this::convertToConsumableVO)
                .collect(Collectors.toList());
        return JsonVO.success(voList);
    }

    private PatientConsumableItemVO convertToConsumableVO(PatientConsumableItemDO item) {
        PatientConsumableItemVO vo = new PatientConsumableItemVO();
        vo.setId(item.getId());
        vo.setBusNo(item.getBusNo());
        vo.setDeviceReqId(item.getDeviceReqId());
        vo.setStatusEnum(item.getStatusEnum());
        vo.setBasedOnTable(item.getBasedOnTable());
        vo.setBasedOnId(item.getBasedOnId());
        vo.setNotPerformedReasonEnum(item.getNotPerformedReasonEnum());
        vo.setDispenseCategoryEnum(item.getDispenseCategoryEnum());
        vo.setQuantity(item.getQuantity());
        vo.setUnitCode(item.getUnitCode());
        vo.setLotNumber(item.getLotNumber());
        vo.setDeviceDefId(item.getDeviceDefId());
        vo.setPatientId(item.getPatientId());
        vo.setEncounterId(item.getEncounterId());
        vo.setPerformerId(item.getPerformerId());
        vo.setLocationId(item.getLocationId());
        vo.setPartTypeCode(item.getPartTypeCode());
        vo.setDispenseQuantity(item.getDispenseQuantity());
        vo.setPreparerId(item.getPreparerId());
        vo.setPrepareTime(item.getPrepareTime());
        vo.setDispenseTime(item.getDispenseTime());
        vo.setLimitTime(item.getLimitTime());
        vo.setPlannedDispenseTime(item.getPlannedDispenseTime());
        vo.setDesLocationId(item.getDesLocationId());
        vo.setRecPractitionerId(item.getRecPractitionerId());
        vo.setSupportInfo(item.getSupportInfo());
        vo.setUsageInstruction(item.getUsageInstruction());
        vo.setDispenseFrequencyCode(item.getDispenseFrequencyCode());
        vo.setTraceNo(item.getTraceNo());
        vo.setProcedureId(item.getProcedureId());
        return vo;
    }

    private ExecutionRecordVO convertToExecutionRecordVO(WkfServiceRequest record) {
        if (record == null) {
            return null;
        }
        ExecutionRecordVO vo = new ExecutionRecordVO();
        vo.setId(record.getId());
        vo.setBus_no(record.getBusNo());
        vo.setPrescription_no(record.getPrescriptionNo());
        vo.setBased_on_table(record.getBasedOnTable());
        vo.setBased_on_id(record.getBasedOnId());
        vo.setStatus_enum(record.getStatusEnum());
        vo.setStatus_name(getStatusName(record.getStatusEnum()));
        vo.setIntent_enum(record.getIntentEnum());
        vo.setCategory_enum(record.getCategoryEnum());
        vo.setCategory_name(getCategoryName(record.getCategoryEnum()));
        vo.setPriority_enum(record.getPriorityEnum());
        vo.setPriority_name(getPriorityName(record.getPriorityEnum()));
        vo.setPerform_flag(record.getPerformFlag());
        vo.setActivity_id(record.getActivityId());
        vo.setItem_name(record.getBasedOnId() != null ? "项目" + record.getBasedOnId() : "未知项目");
        vo.setQuantity(record.getQuantity());
        vo.setUnit_code(record.getUnitCode());
        vo.setUnit_name(getUnitName(record.getUnitCode()));
        vo.setPatient_id(record.getPatientId());
        vo.setEncounter_id(record.getEncounterId());
        vo.setAuthored_time(record.getAuthoredTime() != null ?
                record.getAuthoredTime().format(DATE_TIME_FORMATTER) : null);
        vo.setRequester_id(record.getRequesterId());
        vo.setRequester_name(record.getRequesterId() != null ? "医生" + record.getRequesterId() : null);
        vo.setPerformer_type_code(record.getPerformerTypeCode());
        vo.setPerformer_id(record.getPerformerId());
        vo.setPerformer_name(record.getPerformerId() != null ? "护士" + record.getPerformerId() : null);
        vo.setPerformer_check_id(record.getPerformerCheckId());
        vo.setCheck_time(record.getCheckTime() != null ?
                record.getCheckTime().format(DATE_TIME_FORMATTER) : null);
        vo.setLocation_id(record.getLocationId());
        vo.setLocation_name(record.getLocationId() != null ? "科室" + record.getLocationId() : null);
        vo.setOrg_id(record.getOrgId());
        vo.setReason_text(record.getReasonText());
        vo.setOccurrence_start_time(record.getOccurrenceStartTime() != null ?
                record.getOccurrenceStartTime().format(DATE_TIME_FORMATTER) : null);
        vo.setOccurrence_end_time(record.getOccurrenceEndTime() != null ?
                record.getOccurrenceEndTime().format(DATE_TIME_FORMATTER) : null);
        vo.setOccurrence_date(record.getOccurrenceStartTime() != null ?
                record.getOccurrenceStartTime().format(DATE_FORMATTER) : null);
        vo.setContent_json(record.getContentJson());
        vo.setYb_class_enum(record.getYbClassEnum());
        vo.setPrint_count(record.getPrintCount());
        vo.setRemark(record.getRemark());
        vo.setEntrust_content(record.getEntrustContent());
        vo.setExecution_count(record.getExecutionCount());
        vo.setExecution_status(record.getPerformFlag() != null ? record.getPerformFlag() : 0);
        vo.setExecution_status_name(record.getPerformFlag() != null && record.getPerformFlag() == 1 ? "已执行" : "待执行");
        vo.setCreated_at(record.getCreatedAt() != null ?
                record.getCreatedAt().format(DATE_TIME_FORMATTER) : null);
        return vo;
    }

    private PatientQueueVO convertToPatientQueueVO(WkfAppointmentQueue queue) {
        if (queue == null) {
            return null;
        }
        PatientQueueVO vo = new PatientQueueVO();
        vo.setId(queue.getId());
        vo.setAppointment_kind(queue.getAppointmentKind());
        vo.setBus_no(queue.getBusNo());
        vo.setPatient_id(queue.getPatientId());
        vo.setEncounter_id(queue.getEncounterId());
        vo.setOrganization_id(queue.getOrganizationId());
        vo.setDestination_location_id(queue.getDestinationLocationId());
        vo.setDestination_location_name(queue.getDestinationLocationId() != null ? "诊室" + queue.getDestinationLocationId() : null);
        vo.setTriage_level(queue.getTriageLevel());
        vo.setTriage_level_name(getTriageLevelName(queue.getTriageLevel()));
        vo.setTriage_priority_code(queue.getTriagePriorityCode());
        vo.setTriage_time(queue.getTriageTime() != null ?
                queue.getTriageTime().format(DATE_TIME_FORMATTER) : null);
        vo.setDisplay_order(queue.getDisplayOrder());
        vo.setSlot_id(queue.getSlotId());
        vo.setService_type_code(queue.getServiceTypeCode());
        vo.setService_type_name(getServiceTypeName(queue.getServiceTypeCode()));
        vo.setAppointment_type_code(queue.getAppointmentTypeCode());
        vo.setAppointment_type_name(getAppointmentTypeName(queue.getAppointmentTypeCode()));
        vo.setStart_instant(queue.getStartInstant() != null ?
                queue.getStartInstant().format(DATE_TIME_FORMATTER) : null);
        vo.setEnd_instant(queue.getEndInstant() != null ?
                queue.getEndInstant().format(DATE_TIME_FORMATTER) : null);
        vo.setCancel_reason(queue.getCancelReason());
        vo.setCancellation_reason_code(queue.getCancellationReasonCode());
        vo.setCancellation_at(queue.getCancellationAt() != null ?
                queue.getCancellationAt().format(DATE_TIME_FORMATTER) : null);
        vo.setCalled_at(queue.getCalledAt() != null ?
                queue.getCalledAt().format(DATE_TIME_FORMATTER) : null);
        vo.setLast_practitioner_id(queue.getLastPractitionerId());
        vo.setOrigin_channel(queue.getOriginChannel());
        vo.setTriage_status(getTriageStatus(queue));
        vo.setTriage_status_name(getTriageStatusName(getTriageStatus(queue)));
        vo.setBooking_date(queue.getBookingDate() != null ?
                queue.getBookingDate().format(DATE_FORMATTER) : null);
        vo.setCreated_at(queue.getCreatedAt() != null ?
                queue.getCreatedAt().format(DATE_TIME_FORMATTER) : null);
        return vo;
    }

    private void addFailedItem(BatchExecuteResultVO result, String recordId, String errorCode, String reason) {
        BatchExecuteResultVO.FailedItemVO failedItem = new BatchExecuteResultVO.FailedItemVO();
        failedItem.setRecordId(recordId);
        failedItem.setErrorCode(errorCode);
        failedItem.setReason(reason);
        result.getFailedItems().add(failedItem);
    }

    private String getStatusName(String statusEnum) {
        if (statusEnum == null) return "未知";
        switch (statusEnum) {
            case "ACTIVE": return "活动";
            case "COMPLETED": return "已完成";
            case "CANCELLED": return "已取消";
            case "DRAFT": return "草稿";
            default: return statusEnum;
        }
    }

    private String getPriorityName(String priorityEnum) {
        if (priorityEnum == null) return "未知";
        switch (priorityEnum) {
            case "ROUTINE": return "普通";
            case "URGENT": return "紧急";
            case "ASAP": return "特急";
            case "STAT": return "即刻";
            default: return priorityEnum;
        }
    }

    private String getCategoryName(String categoryEnum) {
        if (categoryEnum == null) return "未知";
        switch (categoryEnum) {
            case "MEDICATION": return "药品";
            case "EXAMINATION": return "检查";
            case "TREATMENT": return "治疗";
            case "SURGERY": return "手术";
            case "NURSING": return "护理";
            case "LAB": return "检验";
            case "IMAGING": return "影像";
            case "CONSUMABLE": return "耗材";
            default: return categoryEnum;
        }
    }

    private String getUnitName(String unitCode) {
        if (unitCode == null) return "未知";
        switch (unitCode) {
            case "BOX": return "盒";
            case "BOTTLE": return "瓶";
            case "AMP": return "安瓿";
            case "TABLET": return "片";
            case "CAPSULE": return "粒";
            case "INJECTION": return "支";
            default: return unitCode;
        }
    }

    private String getTriageLevelName(String level) {
        if (level == null) return "未知";
        switch (level) {
            case "LEVEL1": return "一级（危重）";
            case "LEVEL2": return "二级（急症）";
            case "LEVEL3": return "三级（普通）";
            case "LEVEL4": return "四级（非紧急）";
            default: return level;
        }
    }

    private String getServiceTypeName(String code) {
        if (code == null) return "未知";
        switch (code) {
            case "GENERAL": return "普通门诊";
            case "SPECIALIST": return "专家门诊";
            case "EMERGENCY": return "急诊";
            case "CHILDREN": return "儿科";
            case "OBSTETRICS": return "妇产科";
            default: return code;
        }
    }

    private String getAppointmentTypeName(String code) {
        if (code == null) return "未知";
        switch (code) {
            case "NORMAL": return "普通预约";
            case "URGENT": return "加急预约";
            case "VIP": return "VIP预约";
            case "WALK_IN": return "现场挂号";
            default: return code;
        }
    }

    private String getTriageStatus(WkfAppointmentQueue queue) {
        if (queue == null) return "UNKNOWN";
        if (queue.getCancellationAt() != null) return "CANCELLED";
        if (queue.getCalledAt() != null) return "CALLED";
        if (queue.getStartInstant() != null && queue.getStartInstant().isBefore(LocalDateTime.now())) return "IN_PROGRESS";
        return "WAITING";
    }

    private String getTriageStatusName(String status) {
        if (status == null) return "未知";
        switch (status) {
            case "WAITING": return "候诊中";
            case "CALLED": return "已叫号";
            case "IN_PROGRESS": return "就诊中";
            case "CANCELLED": return "已取消";
            default: return status;
        }
    }

    private String getPrintTypeName(String printType) {
        if (printType == null || printType.isEmpty()) return "执行记录";
        switch (printType) {
            case "EXECUTION_RECORD": return "执行记录";
            case "MEDICATION_LIST": return "药品清单";
            case "TREATMENT_LIST": return "治疗清单";
            case "LAB_ORDER": return "检验单";
            case "EXAM_ORDER": return "检查单";
            case "CONSENT_FORM": return "知情同意书";
            default: return printType;
        }
    }
}
