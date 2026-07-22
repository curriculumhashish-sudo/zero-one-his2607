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
#ifndef _TREATMENTSELECTDTO_H_
#define _TREATMENTSELECTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊疗下拉/选择项 DTO
 */
class TreatmentSelectDTO : public oatpp::DTO
{
    DTO_INIT(TreatmentSelectDTO, DTO);
    // 主键
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
    // 名称
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTSELECTDTO_H_
