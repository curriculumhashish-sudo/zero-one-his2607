#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/24

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
#ifndef _DRUG_UNIT_BAND_DTO_H_
#define _DRUG_UNIT_BAND_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DrugUnitBandDTO : public oatpp::DTO
{
    DTO_INIT(DrugUnitBandDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, bandKind, ZH_WORDS_GETTER("drug.field.bandKind"), true);
    API_DTO_FIELD_REQUIRE(String, unitCode, ZH_WORDS_GETTER("drug.field.unitCode"), true);
    API_DTO_FIELD_REQUIRE(String, unitName, ZH_WORDS_GETTER("drug.field.unitName"), false);
    API_DTO_FIELD_REQUIRE(Float64, ratioToSplit, ZH_WORDS_GETTER("drug.field.ratioToSplit"), true);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice, ZH_WORDS_GETTER("drug.field.unitPrice"), false);

  public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->bandKind = "PACKAGE";
        dto->unitCode = "DW003";
        dto->unitName = ZH_WORDS_GETTER("drug.example.purchaseUnitText");
        dto->ratioToSplit = 10.0;
        dto->unitPrice = 15.8;
        return dto;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DRUG_UNIT_BAND_DTO_H_
