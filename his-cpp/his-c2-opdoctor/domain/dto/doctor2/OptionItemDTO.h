/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/08/09

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
#ifndef _OPTION_ITEM_DTO_H_
#define _OPTION_ITEM_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Generic option item used by all four option arrays
// For departmentOptions, all three fields are populated
// For admissionTypeOptions/admissionWayOptions/priorityOptions, only value and label are used
class OptionItemDTO : public oatpp::DTO
{
    DTO_INIT(OptionItemDTO, DTO);

    // Option value (stable code/ID submitted to backend)
    API_DTO_FIELD_REQUIRE(String, value, ZH_WORDS_GETTER("doctor2.field.value"), true);
    // Option display name
    API_DTO_FIELD_REQUIRE(String, label, ZH_WORDS_GETTER("doctor2.field.label"), true);
    // Option code (used for department options; empty string for others)
    API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("doctor2.field.code"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_OPTION_ITEM_DTO_H_
