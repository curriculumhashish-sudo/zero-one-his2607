package com.zeroone.star.ons.mapper;

import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.Constants;
import com.zeroone.star.ons.dto.QueueWithPatientDTO;
import com.zeroone.star.ons.entity.WkfAppointmentQueue;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * 预约队列 Mapper（映射 wkf_appointment_queue 表）
 */
@Mapper
public interface WkfAppointmentQueueMapper extends BaseMapper<WkfAppointmentQueue> {

    /**
     * 分页查询预约队列并 LEFT JOIN adm_patient 带出患者姓名/性别/出生日期。
     * 使用 MyBatis-Plus ${ew.customSqlSegment} 拼接已有 QueryWrapper 的 WHERE + ORDER BY。
     */
    @Select("SELECT q.id, q.tenant_id, q.created_at, q.updated_at, q.created_by, q.updated_by, " +
            "q.is_deleted, q.deleted_at, q.deleted_by, q.version, " +
            "q.appointment_kind, q.bus_no, q.patient_id, q.encounter_id, q.organization_id, " +
            "q.destination_location_id, q.triage_level, q.triage_priority_code, q.triage_time, " +
            "q.display_order, q.slot_id, q.service_type_code, q.appointment_type_code, " +
            "q.start_instant, q.end_instant, q.cancel_reason, q.cancellation_reason_code, " +
            "q.cancellation_at, q.called_at, q.last_practitioner_id, q.origin_channel, " +
            "q.booking_date, " +
            "p.name AS patientName, p.gender_enum AS patientGenderEnum, p.birth_date AS patientBirthDate, " +
            "loc.name AS destinationLocationName " +
            "FROM wkf_appointment_queue q " +
            "LEFT JOIN adm_patient p ON q.patient_id = p.id AND p.is_deleted = 0 " +
            "LEFT JOIN adm_location loc ON q.destination_location_id = loc.id AND loc.is_deleted = 0 " +
            "${ew.customSqlSegment}")
    <P extends IPage<QueueWithPatientDTO>> P selectPageWithPatient(
            P page,
            @Param(Constants.WRAPPER) Wrapper<WkfAppointmentQueue> wrapper);
}
