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
#ifndef _NURSING_ADD_DTO_
    #define _NURSING_ADD_DTO_
    #include "../../GlobalInclude.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

// ============================================================================
// 共享明细项 DTO（也用于体温模块）
// ============================================================================

/**
 * 通用生命体征明细项 DTO（用于 dia_vital_sign_detail 的键值对）。
 * 由体温模块共享使用（detailItems 列表模式）。
 */
class VitalSignDetailItemDTO : public oatpp::DTO
{
    DTO_INIT(VitalSignDetailItemDTO, DTO);
    // 术语体系 code（如 vital_signs_code）
    API_DTO_FIELD_REQUIRE(String, termCode, ZH_WORDS_GETTER("nursing.field.termCode"), true);
    // 字典项 code
    API_DTO_FIELD_REQUIRE(String, itemCode, ZH_WORDS_GETTER("nursing.field.itemCode"), true);
    // 字典项值
    API_DTO_FIELD_REQUIRE(String, itemValue, ZH_WORDS_GETTER("nursing.field.itemValue"), true);
    // 单位
    API_DTO_FIELD_REQUIRE(String, units, ZH_WORDS_GETTER("nursing.field.units"), false);
    // 明细备注
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("nursing.field.detailRemark"), false);
};

// ============================================================================
// 护理记录新增/保存 DTO —— 扁平测量字段
// ============================================================================

/**
 * 护理记录新增/保存 DTO（type_enum = NURSING_RECORD）。
 * 用于新增和修改接口（修改时 id 从 URL 路径获取）。
 *
 * 服务层将每个测量字段映射到 dia_vital_sign_detail 的一行记录
 *（term_code / item_code / item_value / units）。
 */
class NursingAddDTO : public oatpp::DTO
{
    DTO_INIT(NursingAddDTO, DTO);

    // ======================== 主记录元数据 ========================
    // 患者标识
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("nursing.field.patientId"), true);
    // 就诊标识
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("nursing.field.encounterId"), true);
    // 记录时间（格式：yyyy-MM-dd HH:mm:ss）
    API_DTO_FIELD_REQUIRE(String, recordedAt, ZH_WORDS_GETTER("nursing.field.recordedAt"), true);
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
    // 体温，单位：°C
    API_DTO_FIELD_REQUIRE(String, temperature, ZH_WORDS_GETTER("nursing.field.temperature"), false);
    // 脉搏，单位：次/分
    API_DTO_FIELD_REQUIRE(String, pulse, ZH_WORDS_GETTER("nursing.field.pulse"), false);
    // 心率，单位：次/分
    API_DTO_FIELD_REQUIRE(String, heartRate, ZH_WORDS_GETTER("nursing.field.heartRate"), false);
    // 呼吸，单位：次/分
    API_DTO_FIELD_REQUIRE(String, respiration, ZH_WORDS_GETTER("nursing.field.respiration"), false);
    // 血压，单位：mmHg
    API_DTO_FIELD_REQUIRE(String, bloodPressure, ZH_WORDS_GETTER("nursing.field.bloodPressure"), false);
    // 意识状态
    API_DTO_FIELD_REQUIRE(String, consciousness, ZH_WORDS_GETTER("nursing.field.consciousness"), false);
    // 左侧瞳孔大小，单位：mm
    API_DTO_FIELD_REQUIRE(String, pupilLeft, ZH_WORDS_GETTER("nursing.field.pupilLeft"), false);
    // 右侧瞳孔大小，单位：mm
    API_DTO_FIELD_REQUIRE(String, pupilRight, ZH_WORDS_GETTER("nursing.field.pupilRight"), false);
    // 入量名称（如葡萄糖、生理盐水）
    API_DTO_FIELD_REQUIRE(String, intakeName, ZH_WORDS_GETTER("nursing.field.intakeName"), false);
    // 入量体积，单位：ml
    API_DTO_FIELD_REQUIRE(String, intakeVolume, ZH_WORDS_GETTER("nursing.field.intakeVolume"), false);
    // 出量名称（如尿液、引流液）
    API_DTO_FIELD_REQUIRE(String, outputName, ZH_WORDS_GETTER("nursing.field.outputName"), false);
    // 出量体积，单位：ml
    API_DTO_FIELD_REQUIRE(String, outputVolume, ZH_WORDS_GETTER("nursing.field.outputVolume"), false);
    // 出量颜色
    API_DTO_FIELD_REQUIRE(String, outputColor, ZH_WORDS_GETTER("nursing.field.outputColor"), false);
    // 病情观察及护理记录
    API_DTO_FIELD_REQUIRE(String, observation, ZH_WORDS_GETTER("nursing.field.observation"), false);
    // 护士签名
    API_DTO_FIELD_REQUIRE(String, nurseSignature, ZH_WORDS_GETTER("nursing.field.nurseSignature"), false);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_NURSING_ADD_DTO_