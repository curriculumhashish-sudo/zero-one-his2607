package com.zeroone.star.ons.mapper;

import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.Constants;
import com.zeroone.star.ons.dto.ServiceRequestWithNamesDTO;
import com.zeroone.star.ons.entity.WkfServiceRequest;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * 执行记录 Mapper（映射 wkf_service_request 表）
 */
@Mapper
public interface WkfServiceRequestMapper extends BaseMapper<WkfServiceRequest> {

    /**
     * 分页查询执行记录并 LEFT JOIN 多张表带出名称字段：
     * - 项目名称：根据 based_on_table 动态 JOIN adm_healthcare_service 或 med_medication_definition
     * - 申请人/执行人姓名：adm_practitioner
     * - 科室名称：adm_location
     */
    @Select("SELECT r.id, r.tenant_id, r.created_at, r.updated_at, r.created_by, r.updated_by, " +
            "r.is_deleted, r.deleted_at, r.deleted_by, r.version, " +
            "r.bus_no, r.prescription_no, r.based_on_table, r.based_on_id, r.requisition_id, " +
            "r.status_enum, r.intent_enum, r.category_enum, r.group_id, r.priority_enum, " +
            "r.perform_flag, r.activity_id, r.quantity, r.unit_code, r.patient_id, " +
            "r.encounter_id, r.authored_time, r.requester_id, r.performer_type_code, " +
            "r.performer_id, r.performer_check_id, r.check_time, r.location_id, r.org_id, " +
            "r.reason_text, r.contract_id, r.occurrence_start_time, r.occurrence_end_time, " +
            "r.relevant_history_id, r.refund_service_id, r.content_json, r.yb_class_enum, " +
            "r.print_count, r.condition_definition_id, r.encounter_diagnosis_id, " +
            "r.therapy_enum, r.rate_code, r.parent_id, r.generate_source_enum, " +
            "r.remark, r.entrust_content, r.sort_number, r.execution_count, " +
            "COALESCE(hs.name, md.name) AS itemName, " +
            "rp.name AS requesterName, " +
            "pp.name AS performerName, " +
            "loc.name AS locationName " +
            "FROM wkf_service_request r " +
            "LEFT JOIN adm_healthcare_service hs ON r.based_on_table = 'adm_healthcare_service' " +
            "  AND r.based_on_id = hs.id AND hs.is_deleted = 0 " +
            "LEFT JOIN med_medication_definition md ON r.based_on_table = 'med_medication_definition' " +
            "  AND r.based_on_id = md.id AND md.is_deleted = 0 " +
            "LEFT JOIN adm_practitioner rp ON r.requester_id = rp.id AND rp.is_deleted = 0 " +
            "LEFT JOIN adm_practitioner pp ON r.performer_id = pp.id AND pp.is_deleted = 0 " +
            "LEFT JOIN adm_location loc ON r.location_id = loc.id AND loc.is_deleted = 0 " +
            "${ew.customSqlSegment}")
    <P extends IPage<ServiceRequestWithNamesDTO>> P selectPageWithNames(
            P page,
            @Param(Constants.WRAPPER) Wrapper<WkfServiceRequest> wrapper);

    /**
     * 根据患者ID查询执行记录列表（带名称）。
     */
    @Select("SELECT r.id, r.tenant_id, r.created_at, r.updated_at, r.created_by, r.updated_by, " +
            "r.is_deleted, r.deleted_at, r.deleted_by, r.version, " +
            "r.bus_no, r.prescription_no, r.based_on_table, r.based_on_id, r.requisition_id, " +
            "r.status_enum, r.intent_enum, r.category_enum, r.group_id, r.priority_enum, " +
            "r.perform_flag, r.activity_id, r.quantity, r.unit_code, r.patient_id, " +
            "r.encounter_id, r.authored_time, r.requester_id, r.performer_type_code, " +
            "r.performer_id, r.performer_check_id, r.check_time, r.location_id, r.org_id, " +
            "r.reason_text, r.contract_id, r.occurrence_start_time, r.occurrence_end_time, " +
            "r.relevant_history_id, r.refund_service_id, r.content_json, r.yb_class_enum, " +
            "r.print_count, r.condition_definition_id, r.encounter_diagnosis_id, " +
            "r.therapy_enum, r.rate_code, r.parent_id, r.generate_source_enum, " +
            "r.remark, r.entrust_content, r.sort_number, r.execution_count, " +
            "COALESCE(hs.name, md.name) AS itemName, " +
            "rp.name AS requesterName, " +
            "pp.name AS performerName, " +
            "loc.name AS locationName " +
            "FROM wkf_service_request r " +
            "LEFT JOIN adm_healthcare_service hs ON r.based_on_table = 'adm_healthcare_service' " +
            "  AND r.based_on_id = hs.id AND hs.is_deleted = 0 " +
            "LEFT JOIN med_medication_definition md ON r.based_on_table = 'med_medication_definition' " +
            "  AND r.based_on_id = md.id AND md.is_deleted = 0 " +
            "LEFT JOIN adm_practitioner rp ON r.requester_id = rp.id AND rp.is_deleted = 0 " +
            "LEFT JOIN adm_practitioner pp ON r.performer_id = pp.id AND pp.is_deleted = 0 " +
            "LEFT JOIN adm_location loc ON r.location_id = loc.id AND loc.is_deleted = 0 " +
            "WHERE r.is_deleted = 0 AND r.patient_id = #{patientId} " +
            "ORDER BY r.created_at DESC")
    List<ServiceRequestWithNamesDTO> selectListByPatientIdWithNames(@Param("patientId") String patientId);
}
