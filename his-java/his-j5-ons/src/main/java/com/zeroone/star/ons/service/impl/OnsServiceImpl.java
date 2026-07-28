package com.zeroone.star.ons.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.ons.entity.WkfAppointmentQueue;
import com.zeroone.star.ons.entity.WkfServiceRequest;
import com.zeroone.star.ons.mapper.WkfAppointmentQueueMapper;
import com.zeroone.star.ons.mapper.WkfServiceRequestMapper;
import com.zeroone.star.ons.service.IOnsService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.ExecutionRecordVO;
import com.zeroone.star.project.vo.j5.ons.PatientQueueVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.CollectionUtils;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class OnsServiceImpl implements IOnsService {

    @Resource
    private WkfServiceRequestMapper wkfServiceRequestMapper;

    @Resource
    private WkfAppointmentQueueMapper wkfAppointmentQueueMapper;

    private static final DateTimeFormatter DATE_TIME_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<Boolean> execute(ExecuteRequest request) {
        if (request == null) {
            return JsonVO.fail("请求参数不能为空");
        }
        if (CollectionUtils.isEmpty(request.getRecord_ids())) {
            return JsonVO.fail("执行记录ID列表不能为空");
        }
        if (!StringUtils.hasText(request.getExecutor_id())) {
            return JsonVO.fail("执行者ID不能为空");
        }

        int successCount = 0;
        LocalDateTime now = LocalDateTime.now();
        for (String recordId : request.getRecord_ids()) {
            WkfServiceRequest requestDO = wkfServiceRequestMapper.selectById(recordId);
            if (requestDO == null) {
                continue;
            }

            requestDO.setPerformFlag(1);
            requestDO.setPerformerId(request.getExecutor_id());
            requestDO.setCheckTime(now);
            requestDO.setRemark(request.getRemark());
            requestDO.setExecutionCount(requestDO.getExecutionCount() != null ? requestDO.getExecutionCount() + 1 : 1);

            if (wkfServiceRequestMapper.updateById(requestDO) > 0) {
                successCount++;
            }
        }

        if (successCount == 0) {
            return JsonVO.fail("没有可执行的记录");
        }

        return JsonVO.success(true);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<Boolean> cancelExecute(CancelExecuteRequest request) {
        if (request == null) {
            return JsonVO.fail("请求参数不能为空");
        }
        if (!StringUtils.hasText(request.getRecord_id())) {
            return JsonVO.fail("执行记录ID不能为空");
        }
        if (!StringUtils.hasText(request.getCancel_reason())) {
            return JsonVO.fail("取消原因不能为空");
        }

        WkfServiceRequest requestDO = wkfServiceRequestMapper.selectById(request.getRecord_id());
        if (requestDO == null) {
            return JsonVO.fail("服务请求不存在");
        }

        requestDO.setStatusEnum("CANCELLED");
        requestDO.setReasonText(request.getCancel_reason());

        int result = wkfServiceRequestMapper.updateById(requestDO);
        if (result > 0) {
            return JsonVO.success(true);
        }
        return JsonVO.fail("取消失败");
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

        String printContent = buildPrintContent(records);
        return JsonVO.success(printContent);
    }

    private String buildPrintContent(List<WkfServiceRequest> records) {
        StringBuilder sb = new StringBuilder();
        sb.append("========================================\n");
        sb.append("         服务请求打印单\n");
        sb.append("========================================\n");

        for (int i = 0; i < records.size(); i++) {
            WkfServiceRequest record = records.get(i);
            sb.append("\n").append(i + 1).append(". 业务编号：").append(record.getBusNo()).append("\n");
            sb.append("   处方编号：").append(record.getPrescriptionNo()).append("\n");
            sb.append("   患者ID：").append(record.getPatientId()).append("\n");
            sb.append("   就诊ID：").append(record.getEncounterId()).append("\n");
            sb.append("   活动标识：").append(record.getActivityId()).append("\n");
            sb.append("   数量：").append(record.getQuantity()).append(" ").append(record.getUnitCode()).append("\n");
            sb.append("   状态：").append(record.getStatusEnum()).append("\n");
            sb.append("   优先级：").append(record.getPriorityEnum()).append("\n");
            sb.append("   开立时间：").append(record.getAuthoredTime() != null ?
                    record.getAuthoredTime().format(DATE_TIME_FORMATTER) : "未开立").append("\n");
            sb.append("   执行者：").append(record.getPerformerId()).append("\n");
            sb.append("   备注：").append(record.getRemark()).append("\n");
        }

        sb.append("\n========================================\n");
        sb.append("打印时间：").append(LocalDateTime.now().format(DATE_TIME_FORMATTER)).append("\n");
        sb.append("========================================\n");

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
        vo.setIntent_enum(record.getIntentEnum());
        vo.setCategory_enum(record.getCategoryEnum());
        vo.setPriority_enum(record.getPriorityEnum());
        vo.setPerform_flag(record.getPerformFlag());
        vo.setActivity_id(record.getActivityId());
        vo.setQuantity(record.getQuantity());
        vo.setUnit_code(record.getUnitCode());
        vo.setPatient_id(record.getPatientId());
        vo.setEncounter_id(record.getEncounterId());
        vo.setAuthored_time(record.getAuthoredTime() != null ?
                record.getAuthoredTime().format(DATE_TIME_FORMATTER) : null);
        vo.setRequester_id(record.getRequesterId());
        vo.setPerformer_type_code(record.getPerformerTypeCode());
        vo.setPerformer_id(record.getPerformerId());
        vo.setPerformer_check_id(record.getPerformerCheckId());
        vo.setCheck_time(record.getCheckTime() != null ?
                record.getCheckTime().format(DATE_TIME_FORMATTER) : null);
        vo.setLocation_id(record.getLocationId());
        vo.setOrg_id(record.getOrgId());
        vo.setReason_text(record.getReasonText());
        vo.setOccurrence_start_time(record.getOccurrenceStartTime() != null ?
                record.getOccurrenceStartTime().format(DATE_TIME_FORMATTER) : null);
        vo.setOccurrence_end_time(record.getOccurrenceEndTime() != null ?
                record.getOccurrenceEndTime().format(DATE_TIME_FORMATTER) : null);
        vo.setContent_json(record.getContentJson());
        vo.setYb_class_enum(record.getYbClassEnum());
        vo.setPrint_count(record.getPrintCount());
        vo.setRemark(record.getRemark());
        vo.setEntrust_content(record.getEntrustContent());
        vo.setExecution_count(record.getExecutionCount());
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
        vo.setTriage_level(queue.getTriageLevel());
        vo.setTriage_priority_code(queue.getTriagePriorityCode());
        vo.setTriage_time(queue.getTriageTime() != null ?
                queue.getTriageTime().format(DATE_TIME_FORMATTER) : null);
        vo.setDisplay_order(queue.getDisplayOrder());
        vo.setSlot_id(queue.getSlotId());
        vo.setService_type_code(queue.getServiceTypeCode());
        vo.setAppointment_type_code(queue.getAppointmentTypeCode());
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
        vo.setBooking_date(queue.getBookingDate() != null ?
                queue.getBookingDate().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) : null);
        vo.setCreated_at(queue.getCreatedAt() != null ?
                queue.getCreatedAt().format(DATE_TIME_FORMATTER) : null);
        return vo;
    }
}