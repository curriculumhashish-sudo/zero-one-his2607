#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/19 10:00:00

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
#ifndef _NURSING_DETAIL_DTO_
    #define _NURSING_DETAIL_DTO_
    #include "../../GlobalInclude.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

// ============================================================================
// 护理查询/展示 DTO —— 扁平测量字段 + 患者人口学字段
// ============================================================================

/**
 * 护理记录详情 DTO —— 查询/展示时返回的完整记录。
 *
 * 包含审计字段 + 主记录元数据 + 测量字段 + 患者信息。
 * 患者信息由 Service 层通过 JOIN 填充：
 *   adm_patient                → 姓名、性别、年龄、病历号
 *   adm_encounter              → 入院时间、出院时间
 *   adm_encounter_location     → 筛选条件：form_enum=ROOM/BED，status_enum=ACTIVE
 *   adm_location               → 病室名称、床号
 */
class NursingDetailDTO : public oatpp::DTO
{
    DTO_INIT(NursingDetailDTO, DTO);

    // ======================== 审计字段 ========================
    // 主键 ID
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("nursing.field.id"), true);
    // 创建人
    API_DTO_FIELD_REQUIRE(String, createdBy, ZH_WORDS_GETTER("nursing.field.createdBy"), true);
    // 创建时间
    API_DTO_FIELD_REQUIRE(String, createdAt, ZH_WORDS_GETTER("nursing.field.createdAt"), true);
    // 最后更新人
    API_DTO_FIELD_REQUIRE(String, updatedBy, ZH_WORDS_GETTER("nursing.field.updatedBy"), false);
    // 最后更新时间
    API_DTO_FIELD_REQUIRE(String, updatedAt, ZH_WORDS_GETTER("nursing.field.updatedAt"), false);

    // ======================== 主记录元数据 ========================
    // 患者标识
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("nursing.field.patientId"), true);
    // 就诊标识
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("nursing.field.encounterId"), true);
    // 记录时间
    API_DTO_FIELD_REQUIRE(String, recordedAt, ZH_WORDS_GETTER("nursing.field.recordedAt"), true);
    // 单据类型：NURSING_RECORD
    API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("nursing.field.typeEnum"), true);
    // 业务单号
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("nursing.field.busNo"), true);
    // 位置/科室标识
    API_DTO_FIELD_REQUIRE(String, locationId, ZH_WORDS_GETTER("nursing.field.locationId"), false);
    // 婴儿编号
    API_DTO_FIELD_REQUIRE(Int32, babyNo, ZH_WORDS_GETTER("nursing.field.babyNo"), false);
    // 记录人标识
    API_DTO_FIELD_REQUIRE(String, recorderId, ZH_WORDS_GETTER("nursing.field.recorderId"), true);
    // 备注
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("nursing.field.remark"), false);

    // ======================== 护理测量字段 ========================
    // 体温（°C）
    API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("nursing.field.temperature"), false);
    // 脉搏（次/分）
    API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("nursing.field.pulse"), false);
    // 心率（次/分）
    API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("nursing.field.heartRate"), false);
    // 呼吸（次/分）
    API_DTO_FIELD_REQUIRE(String, respiration, ZH_WORDS_GETTER("nursing.field.respiration"), false);
    // 血压（mmHg）
    API_DTO_FIELD_REQUIRE(String, bloodPressure, ZH_WORDS_GETTER("nursing.field.bloodPressure"), false);
    // 意识状态
    API_DTO_FIELD_REQUIRE(String, consciousness, ZH_WORDS_GETTER("nursing.field.consciousness"), false);
    // 左侧瞳孔大小（mm）
    API_DTO_FIELD_REQUIRE(String, pupilLeft, ZH_WORDS_GETTER("nursing.field.pupilLeft"), false);
    // 右侧瞳孔大小（mm）
    API_DTO_FIELD_REQUIRE(String, pupilRight, ZH_WORDS_GETTER("nursing.field.pupilRight"), false);
    // 入量名称
    API_DTO_FIELD_REQUIRE(String, intakeName, ZH_WORDS_GETTER("nursing.field.intakeName"), false);
    // 入量体积（ml）
    API_DTO_FIELD_REQUIRE(String, intakeVolume, ZH_WORDS_GETTER("nursing.field.intakeVolume"), false);
    // 出量名称
    API_DTO_FIELD_REQUIRE(String, outputName, ZH_WORDS_GETTER("nursing.field.outputName"), false);
    // 出量体积（ml）
    API_DTO_FIELD_REQUIRE(String, outputVolume, ZH_WORDS_GETTER("nursing.field.outputVolume"), false);
    // 出量颜色
    API_DTO_FIELD_REQUIRE(String, outputColor, ZH_WORDS_GETTER("nursing.field.outputColor"), false);
    // 病情观察及护理记录
    API_DTO_FIELD_REQUIRE(String, observation, ZH_WORDS_GETTER("nursing.field.observation"), false);
    // 护士签名
    API_DTO_FIELD_REQUIRE(String, nurseSignature, ZH_WORDS_GETTER("nursing.field.nurseSignature"), false);

    // ======================== 患者人口学字段（来自 JOIN） ========================
    // 患者姓名（来自 adm_patient.name）
    API_DTO_FIELD_REQUIRE(String, patientName, ZH_WORDS_GETTER("nursing.field.patientName"), false);
    // 患者性别（来自 adm_patient.gender_enum）
    API_DTO_FIELD_REQUIRE(String, patientGender, ZH_WORDS_GETTER("nursing.field.patientGender"), false);
    // 患者年龄（由 adm_patient.birth_date 计算得出）
    API_DTO_FIELD_REQUIRE(Int32, patientAge, ZH_WORDS_GETTER("nursing.field.patientAge"), false);
    // 病室（来自 adm_location.name，通过 adm_encounter_location 关联，form_enum=ROOM）
    API_DTO_FIELD_REQUIRE(String, ward, ZH_WORDS_GETTER("nursing.field.ward"), false);
    // 床号（来自 adm_location.name，通过 adm_encounter_location 关联，form_enum=BED）
    API_DTO_FIELD_REQUIRE(String, bedNo, ZH_WORDS_GETTER("nursing.field.bedNo"), false);
    // 病历号（来自 adm_patient.bus_no）
    API_DTO_FIELD_REQUIRE(String, medicalRecordNo, ZH_WORDS_GETTER("nursing.field.medicalRecordNo"), false);
    // 入院时间（来自 adm_encounter.start_time）
    API_DTO_FIELD_REQUIRE(String, admissionTime, ZH_WORDS_GETTER("nursing.field.admissionTime"), false);
    // 出院时间（来自 adm_encounter.end_time）
    API_DTO_FIELD_REQUIRE(String, dischargeTime, ZH_WORDS_GETTER("nursing.field.dischargeTime"), false);
};

/**
 * 护理记录分页 DTO
 */
class NursingPageDTO : public PageDTO<NursingDetailDTO::Wrapper>
{
    DTO_INIT(NursingPageDTO, PageDTO<NursingDetailDTO::Wrapper>);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_NURSING_DETAIL_DTO_