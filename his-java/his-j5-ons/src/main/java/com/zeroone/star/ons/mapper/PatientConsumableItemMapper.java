package com.zeroone.star.ons.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.ons.entity.PatientConsumableItemDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：患者耗材项目Mapper（映射wkf_device_dispense表）
 * </p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface PatientConsumableItemMapper extends BaseMapper<PatientConsumableItemDO> {

    /**
     * 根据患者ID查询耗材项目列表（指定字段，避免select *，LEFT JOIN设备定义表获取设备名称）
     */
    @Select("SELECT d.id, d.bus_no, d.device_req_id, d.status_enum, d.based_on_table, d.based_on_id, " +
            "d.not_performed_reason_enum, d.dispense_category_enum, d.quantity, d.unit_code, " +
            "d.lot_number, d.device_def_id, dev.name as device_name, d.patient_id, d.encounter_id, d.performer_id, " +
            "d.location_id, d.part_type_code, d.dispense_quantity, d.preparer_id, d.prepare_time, " +
            "d.dispense_time, d.limit_time, d.planned_dispense_time, d.des_location_id, " +
            "d.rec_practitioner_id, d.support_info, d.usage_instruction, d.dispense_frequency_code, " +
            "d.trace_no, d.procedure_id " +
            "FROM wkf_device_dispense d " +
            "LEFT JOIN adm_device_definition dev ON d.device_def_id = dev.id AND dev.is_deleted = 0 " +
            "WHERE d.patient_id = #{patientId} AND d.is_deleted = 0 " +
            "ORDER BY d.created_at DESC")
    List<PatientConsumableItemDO> selectByPatientId(@Param("patientId") String patientId);

    /**
     * 根据患者ID和就诊ID查询耗材项目列表（指定字段，避免select *，LEFT JOIN设备定义表获取设备名称）
     */
    @Select("SELECT d.id, d.bus_no, d.device_req_id, d.status_enum, d.based_on_table, d.based_on_id, " +
            "d.not_performed_reason_enum, d.dispense_category_enum, d.quantity, d.unit_code, " +
            "d.lot_number, d.device_def_id, dev.name as device_name, d.patient_id, d.encounter_id, d.performer_id, " +
            "d.location_id, d.part_type_code, d.dispense_quantity, d.preparer_id, d.prepare_time, " +
            "d.dispense_time, d.limit_time, d.planned_dispense_time, d.des_location_id, " +
            "d.rec_practitioner_id, d.support_info, d.usage_instruction, d.dispense_frequency_code, " +
            "d.trace_no, d.procedure_id " +
            "FROM wkf_device_dispense d " +
            "LEFT JOIN adm_device_definition dev ON d.device_def_id = dev.id AND dev.is_deleted = 0 " +
            "WHERE d.patient_id = #{patientId} AND d.encounter_id = #{encounterId} AND d.is_deleted = 0 " +
            "ORDER BY d.created_at DESC")
    List<PatientConsumableItemDO> selectByPatientIdAndEncounterId(
            @Param("patientId") String patientId,
            @Param("encounterId") String encounterId);
}
