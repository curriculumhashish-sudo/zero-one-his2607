#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 10:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _TEMPERATURE_DTO_
    #define _TEMPERATURE_DTO_
    #include "../../GlobalInclude.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

// ============================================================================
// TemperatureTimeSlotDTO — 单时间段测量数据
// ============================================================================

/**
 * 体温单中一个时间段的测量数据。
 * 每天有7个时间段：02:00、04:00、06:00、10:00、14:00、18:00、22:00。
 */
class TemperatureTimeSlotDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureTimeSlotDTO, DTO);
    // 时间段标识："02:00" / "04:00" / "06:00" / "10:00" / "14:00" / "18:00" / "22:00"
    API_DTO_FIELD_REQUIRE(String, timeSlot, ZH_WORDS_GETTER("temperature.field.timeSlot"), true);
    // 体温（°C）
    API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("temperature.field.temperature"), false);
    // 脉搏（次/分）
    API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("temperature.field.pulse"), false);
    // 呼吸（次/分）
    API_DTO_FIELD_REQUIRE(String, respiration, ZH_WORDS_GETTER("temperature.field.respiration"), false);
    // 心率（次/分）
    API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("temperature.field.heartRate"), false);
    // 收缩压（mmHg）
    API_DTO_FIELD_REQUIRE(String, systolicPressure, ZH_WORDS_GETTER("temperature.field.systolicPressure"), false);
    // 舒张压（mmHg）
    API_DTO_FIELD_REQUIRE(String, diastolicPressure, ZH_WORDS_GETTER("temperature.field.diastolicPressure"), false);
    // 是否物理降温
    API_DTO_FIELD_REQUIRE(Boolean, physicalCooling, ZH_WORDS_GETTER("temperature.field.physicalCooling"), false);
    // 物理降温后体温（°C）
    API_DTO_FIELD_REQUIRE(String,
                          tempAfterPhysicalCooling,
                          ZH_WORDS_GETTER("temperature.field.tempAfterPhysicalCooling"),
                          false);
    // 是否药物降温
    API_DTO_FIELD_REQUIRE(Boolean, drugCooling, ZH_WORDS_GETTER("temperature.field.drugCooling"), false);
    // 药物降温后体温（°C）
    API_DTO_FIELD_REQUIRE(String,
                          tempAfterDrugCooling,
                          ZH_WORDS_GETTER("temperature.field.tempAfterDrugCooling"),
                          false);
    // 是否体温不升
    API_DTO_FIELD_REQUIRE(Boolean, tempNotRising, ZH_WORDS_GETTER("temperature.field.tempNotRising"), false);
    // 是否出院
    API_DTO_FIELD_REQUIRE(Boolean, discharge, ZH_WORDS_GETTER("temperature.field.discharge"), false);
    // 出院时间（yyyy-MM-dd HH:mm:ss）
    API_DTO_FIELD_REQUIRE(String, dischargeTime, ZH_WORDS_GETTER("temperature.field.dischargeTime"), false);
    // 特殊事件类型：转科 / 死亡 / 分娩 / 手术 / 外出检查 / 拒测体温
    API_DTO_FIELD_REQUIRE(String, specialEvent, ZH_WORDS_GETTER("temperature.field.specialEvent"), false);
    // 特殊事件时间（yyyy-MM-dd HH:mm:ss）
    API_DTO_FIELD_REQUIRE(String, specialEventTime, ZH_WORDS_GETTER("temperature.field.specialEventTime"), false);
};

// ============================================================================
// TemperatureSupplementaryDTO — 补充记录（底部区域）
// ============================================================================

/**
 * 显示在体温单下方的补充记录。
 * 每张体温单对应一组（所有时间段共享）。
 */
class TemperatureSupplementaryDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureSupplementaryDTO, DTO);
    // 体重（KG）
    API_DTO_FIELD_REQUIRE(String, weight, ZH_WORDS_GETTER("temperature.field.weight"), false);
    // 排尿次数（次）
    API_DTO_FIELD_REQUIRE(String, urinationTimes, ZH_WORDS_GETTER("temperature.field.urinationTimes"), false);
    // 腰围（cm）
    API_DTO_FIELD_REQUIRE(String, waist, ZH_WORDS_GETTER("temperature.field.waist"), false);
    // 排便次数（次）
    API_DTO_FIELD_REQUIRE(String, defecationTimes, ZH_WORDS_GETTER("temperature.field.defecationTimes"), false);
    // 身高（cm）
    API_DTO_FIELD_REQUIRE(String, height, ZH_WORDS_GETTER("temperature.field.height"), false);
    // 入量（ml）
    API_DTO_FIELD_REQUIRE(String, intake, ZH_WORDS_GETTER("temperature.field.intake"), false);
    // 皮试阳性结果
    API_DTO_FIELD_REQUIRE(String, skinTestPositive, ZH_WORDS_GETTER("temperature.field.skinTestPositive"), false);
    // 尿量（ml）
    API_DTO_FIELD_REQUIRE(String, urineVolume, ZH_WORDS_GETTER("temperature.field.urineVolume"), false);
    // 其他液量（ml）
    API_DTO_FIELD_REQUIRE(String, other, ZH_WORDS_GETTER("temperature.field.other"), false);
};

// ============================================================================
// TemperatureSaveDTO — 保存/更新请求体
// ============================================================================

/**
 * 整张体温单的保存请求。
 * 包含主表元数据、7个时间段和补充记录。
 * 使用插入或更新：若不存在则插入，若同一患者+日期已存在则更新。
 */
class TemperatureSaveDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureSaveDTO, DTO);
    // 患者ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("temperature.field.patientId"), true);
    // 就诊ID
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("temperature.field.encounterId"), true);
    // 记录日期（yyyy-MM-dd）
    API_DTO_FIELD_REQUIRE(String, recordedDate, ZH_WORDS_GETTER("temperature.field.recordedDate"), true);
    // 位置/科室ID
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("temperature.field.locationId"), false);
    // 记录人ID
    API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("temperature.field.recorderId"), true);
    // 业务编号
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("temperature.field.busNo"), false);
    // 备注
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("temperature.field.remark"), false);
    // 时间段数据（一天7条记录）
    DTO_FIELD(List<TemperatureTimeSlotDTO::Wrapper>, timeSlots);
    DTO_FIELD_INFO(timeSlots)
    {
        info->description = ZH_WORDS_GETTER("temperature.field.timeSlots");
    }
    // 补充记录
    API_DTO_FIELD_REQUIRE(TemperatureSupplementaryDTO::Wrapper,
                          supplementary,
                          ZH_WORDS_GETTER("temperature.field.supplementary"),
                          false);
};

// ============================================================================
// TemperatureDetailDTO — 查询响应（= 保存字段 + 审计字段 + 患者信息）
// ============================================================================

/**
 * 按日期查询返回的完整体温单详情。
 * 扩展了保存字段，增加审计字段和从 adm_patient、adm_encounter、
 * adm_encounter_location、adm_location 表关联查询的患者人口学信息。
 */
class TemperatureDetailDTO : public oatpp::DTO
{
    DTO_INIT(TemperatureDetailDTO, DTO);

    // ======================== 审计字段 ========================
    // 主键ID
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("temperature.field.id"), true);
    // 创建人
    API_DTO_FIELD_REQUIRE(String, createdBy, ZH_WORDS_GETTER("temperature.field.createdBy"), true);
    // 创建时间
    API_DTO_FIELD_REQUIRE(String, createdAt, ZH_WORDS_GETTER("temperature.field.createdAt"), true);
    // 更新人
    API_DTO_FIELD_REQUIRE(String, updatedBy, ZH_WORDS_GETTER("temperature.field.updatedBy"), false);
    // 更新时间
    API_DTO_FIELD_REQUIRE(String, updatedAt, ZH_WORDS_GETTER("temperature.field.updatedAt"), false);

    // ======================== 主表元数据 ========================
    // 患者ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("temperature.field.patientId"), true);
    // 就诊ID
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("temperature.field.encounterId"), true);
    // 记录日期（yyyy-MM-dd）
    API_DTO_FIELD_REQUIRE(String, recordedDate, ZH_WORDS_GETTER("temperature.field.recordedDate"), true);
    // 文档类型：TEMPERATURE_SHEET
    API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("temperature.field.typeEnum"), true);
    // 位置/科室ID
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("temperature.field.locationId"), false);
    // 记录人ID
    API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("temperature.field.recorderId"), true);
    // 业务编号
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("temperature.field.busNo"), false);
    // 备注
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("temperature.field.remark"), false);

    // ======================== 时间段数据 ========================
    DTO_FIELD(List<TemperatureTimeSlotDTO::Wrapper>, timeSlots);
    DTO_FIELD_INFO(timeSlots)
    {
        info->description = ZH_WORDS_GETTER("temperature.field.timeSlots");
    }

    // ======================== 补充记录 ========================
    API_DTO_FIELD_REQUIRE(TemperatureSupplementaryDTO::Wrapper,
                          supplementary,
                          ZH_WORDS_GETTER("temperature.field.supplementary"),
                          false);

    // ======================== 患者信息（关联查询） ========================
    // 患者姓名（来自 adm_patient.name）
    API_DTO_FIELD_REQUIRE(String, patientName, ZH_WORDS_GETTER("temperature.field.patientName"), false);
    // 患者性别（来自 adm_patient.gender_enum）
    API_DTO_FIELD_REQUIRE(String, patientGender, ZH_WORDS_GETTER("temperature.field.patientGender"), false);
    // 患者年龄（由 adm_patient.birth_date 计算）
    API_DTO_FIELD_REQUIRE(Int32, patientAge, ZH_WORDS_GETTER("temperature.field.patientAge"), false);
    // 病房名称（来自 adm_location WHERE form_enum=ROOM）
    API_DTO_FIELD_REQUIRE(String, ward, ZH_WORDS_GETTER("temperature.field.ward"), false);
    // 床号（来自 adm_location WHERE form_enum=BED）
    API_DTO_FIELD_REQUIRE(String, bedNo, ZH_WORDS_GETTER("temperature.field.bedNo"), false);
    // 病历号（来自 adm_patient.bus_no）
    API_DTO_FIELD_REQUIRE(String, medicalRecordNo, ZH_WORDS_GETTER("temperature.field.medicalRecordNo"), false);
    // 入院时间（来自 adm_encounter.start_time）
    API_DTO_FIELD_REQUIRE(String, admissionTime, ZH_WORDS_GETTER("temperature.field.admissionTime"), false);
    // 出院时间（来自 adm_encounter.end_time）
    API_DTO_FIELD_REQUIRE(String, dischargeTime, ZH_WORDS_GETTER("temperature.field.dischargeTime"), false);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_TEMPERATURE_DTO_