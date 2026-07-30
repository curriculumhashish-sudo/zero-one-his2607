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

/** 选择诊疗列表项 DTO。 */
class TreatmentSelectDTO : public oatpp::DTO
{
    DTO_INIT(TreatmentSelectDTO, DTO);
    // 诊疗主键。
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
    // 诊疗业务编码。
    API_DTO_FIELD_REQUIRE(String, busNo, ZH_WORDS_GETTER("treatment.field.bus-no"), true);
    // 诊疗名称。
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
    // 目录分类编码和字典文本。
    API_DTO_FIELD_REQUIRE(String, categoryCode, ZH_WORDS_GETTER("treatment.field.category-code"), true);
    DTO_FIELD(String, categoryCodeDictText, "categoryCode_dictText");
    DTO_FIELD_INFO(categoryCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("treatment.field.category-code-text");
        info->required = true;
    }
    // 机构名称。
    DTO_FIELD(String, orgIdDictText, "orgId_dictText");
    DTO_FIELD_INFO(orgIdDictText)
    {
        info->description = ZH_WORDS_GETTER("treatment.field.org-id-text");
        info->required = false;
    }
    // 价格、规格和厂家。
    API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("treatment.field.retail-price"), true);
    API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("treatment.field.specification"), false);
    API_DTO_FIELD_REQUIRE(String, manufacturerText, ZH_WORDS_GETTER("treatment.field.manufacturer-text"), false);
    // 财务分类编码和字典文本。
    API_DTO_FIELD_REQUIRE(String, itemTypeCode, ZH_WORDS_GETTER("treatment.field.item-type-code"), false);
    DTO_FIELD(String, itemTypeCodeDictText, "itemTypeCode_dictText");
    DTO_FIELD_INFO(itemTypeCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("treatment.field.item-type-code-text");
        info->required = false;
    }
    // 计价单位编码和字典文本。
    API_DTO_FIELD_REQUIRE(String, packageUnitCode, ZH_WORDS_GETTER("treatment.field.package-unit-code"), false);
    DTO_FIELD(String, packageUnitCodeDictText, "packageUnitCode_dictText");
    DTO_FIELD_INFO(packageUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("treatment.field.package-unit-code-text");
        info->required = false;
    }
    // 默认门诊单位编码和字典文本。
    API_DTO_FIELD_REQUIRE(
        String,
        defEncounterUnitCode,
        ZH_WORDS_GETTER("treatment.field.def-encounter-unit-code"),
        false);
    DTO_FIELD(String, defEncounterUnitCodeDictText, "defEncounterUnitCode_dictText");
    DTO_FIELD_INFO(defEncounterUnitCodeDictText)
    {
        info->description = ZH_WORDS_GETTER("treatment.field.def-encounter-unit-code-text");
        info->required = false;
    }
    // 医保目录编号。
    API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("treatment.field.yb-no"), false);
    // 状态数值和枚举文本。
    API_DTO_FIELD_REQUIRE(Int32, statusEnum, ZH_WORDS_GETTER("treatment.field.status-enum"), true);
    DTO_FIELD(String, statusEnumText, "statusEnum_enumText");
    DTO_FIELD_INFO(statusEnumText)
    {
        info->description = ZH_WORDS_GETTER("treatment.field.status-enum-text");
        info->required = true;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTSELECTDTO_H_
