#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
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
#ifndef _TREATMENTSTATUSDTO_H_
#define _TREATMENTSTATUSDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 批量启/停用诊疗 DTO
 */
class TreatmentStatusDTO : public oatpp::DTO
{
    DTO_INIT(TreatmentStatusDTO, DTO);
    // 要修改状态的记录主键列表
    API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("treatment.field.ids"), true);
    // 目标状态：1-启用 0-停用
    API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("treatment.field.status"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTSTATUSDTO_H_
