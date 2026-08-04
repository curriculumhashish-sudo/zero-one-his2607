#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/08/01 10:00:00

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
#ifndef _PHARMADDDTO_H_
#define _PHARMADDDTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PayloadDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * PharmAddDTO - Request DTO for adding a pharmacy/warehouse.
 */
class PharmAddDTO : public oatpp::DTO
{
    DTO_INIT(PharmAddDTO, DTO);

    // Required fields
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, formEnum, ZH_WORDS_GETTER("pharm.field.formEnum"), true);

    // Optional fields
    API_DTO_FIELD_DEFAULT(oatpp::String, busNo, ZH_WORDS_GETTER("pharm.field.busNo"));
    API_DTO_FIELD_DEFAULT(oatpp::String, organizationId, ZH_WORDS_GETTER("pharm.field.organizationId"));
    API_DTO_FIELD_DEFAULT(oatpp::Int32, displayOrder, ZH_WORDS_GETTER("pharm.field.displayOrder"));
    API_DTO_FIELD_DEFAULT(oatpp::String, pyStr, ZH_WORDS_GETTER("pharm.field.pyStr"));
    API_DTO_FIELD_DEFAULT(oatpp::String, wbStr, ZH_WORDS_GETTER("pharm.field.wbStr"));
    API_DTO_FIELD_DEFAULT(oatpp::String, parentId, ZH_WORDS_GETTER("pharm.field.parentId"));

    // JWT payload injection
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate()
    {
        if (!name || name->empty())
            return "name is required";
        if (!formEnum || formEnum->empty())
            return "formEnum is required";
        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMADDDTO_H_
