/*
 Copyright Zero One Star. All rights reserved.

 停嘱请求 DTO

 @Author: nick1484065161
 @Date: 2026/07/20 00:00:00

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
#pragma once

// 防止头文件被重复包含
#ifndef _TCM_PRESCRIPTION_STOP_DTO_H_
#define _TCM_PRESCRIPTION_STOP_DTO_H_

// 引入项目通用定义和 DTO 宏
#include "../../GlobalInclude.h"

// 开始生成 Oatpp DTO 相关代码
#include OATPP_CODEGEN_BEGIN(DTO)

// 定义停嘱请求 DTO
class TcmPrescriptionStopDTO : public oatpp::DTO
{
    // 初始化当前 DTO 类型
    DTO_INIT(TcmPrescriptionStopDTO, DTO);

    // 当前住院就诊记录 ID
    API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("tcm-prescription.field.encounter-id"), true, "335444239397687296");

    // 当前患者 ID
    API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("tcm-prescription.field.patient-id"), true, "335441797981409280");

    // 需要停嘱的处方编号列表
    API_DTO_FIELD(List<String>, prescriptionNos, ZH_WORDS_GETTER("tcm-prescription.field.prescription-nos"), true, {});

    // 停嘱时间，可以不传；不传时由后端使用当前时间
    API_DTO_FIELD(String, stopTime, ZH_WORDS_GETTER("tcm-prescription.field.stop-time"), false, "2026-07-20 11:00:00");
};

// 结束生成 Oatpp DTO 相关代码
#include OATPP_CODEGEN_END(DTO)

// 结束头文件保护
#endif
