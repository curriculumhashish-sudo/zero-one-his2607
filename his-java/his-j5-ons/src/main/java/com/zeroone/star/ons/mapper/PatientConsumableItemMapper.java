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
     * 根据患者ID查询耗材项目列表（指定字段，避免select *）
     */
    @Select("SELECT id, bus_no, device_req_id, status_enum, based_on_table, based_on_id, " +
            "not_performed_reason_enum, dispense_category_enum, quantity, unit_code, " +
            "lot_number, device_def_id, patient_id, encounter_id, performer_id, " +
            "location_id, part_type_code, dispense_quantity, preparer_id, prepare_time, " +
            "dispense_time, limit_time, planned_dispense_time, des_location_id, " +
            "rec_practitioner_id, support_info, usage_instruction, dispense_frequency_code, " +
            "trace_no, procedure_id " +
            "FROM wkf_device_dispense WHERE patient_id = #{patientId} AND is_deleted = 0 " +
            "ORDER BY created_at DESC")
    List<PatientConsumableItemDO> selectByPatientId(@Param("patientId") String patientId);

    /**
     * 根据患者ID和就诊ID查询耗材项目列表（指定字段，避免select *）
     */
    @Select("SELECT id, bus_no, device_req_id, status_enum, based_on_table, based_on_id, " +
            "not_performed_reason_enum, dispense_category_enum, quantity, unit_code, " +
            "lot_number, device_def_id, patient_id, encounter_id, performer_id, " +
            "location_id, part_type_code, dispense_quantity, preparer_id, prepare_time, " +
            "dispense_time, limit_time, planned_dispense_time, des_location_id, " +
            "rec_practitioner_id, support_info, usage_instruction, dispense_frequency_code, " +
            "trace_no, procedure_id " +
            "FROM wkf_device_dispense WHERE patient_id = #{patientId} AND encounter_id = #{encounterId} AND is_deleted = 0 " +
            "ORDER BY created_at DESC")
    List<PatientConsumableItemDO> selectByPatientIdAndEncounterId(
            @Param("patientId") String patientId,
            @Param("encounterId") String encounterId);
}
