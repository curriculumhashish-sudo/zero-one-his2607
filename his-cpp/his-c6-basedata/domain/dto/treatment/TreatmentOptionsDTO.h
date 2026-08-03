#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/08/03

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
#ifndef _TREATMENT_OPTIONS_DTO_H_
#define _TREATMENT_OPTIONS_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/** 新版诊疗选择列表单行数据，字段统一使用 lowerCamelCase。 */
class TreatmentOptionV2DTO : public oatpp::DTO
{
	DTO_INIT(TreatmentOptionV2DTO, DTO);

	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("treatment.field.code"), true);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
	API_DTO_FIELD_REQUIRE(String, categoryCode, ZH_WORDS_GETTER("treatment.field.category-code"), true);
	DTO_FIELD(String, categoryCodeDictText, "categoryCode_dictText");
	DTO_FIELD_INFO(categoryCodeDictText)
	{
		info->description = ZH_WORDS_GETTER("treatment.field.category-code-text");
		info->required = false;
	}
	DTO_FIELD(String, orgIdDictText, "orgId_dictText");
	DTO_FIELD_INFO(orgIdDictText)
	{
		info->description = ZH_WORDS_GETTER("treatment.field.org-id-text");
		info->required = false;
	}
	API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("treatment.field.retail-price"), false);
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("treatment.field.specification"), false);
	API_DTO_FIELD_REQUIRE(String, manufacturerText, ZH_WORDS_GETTER("treatment.field.manufacturer-text"), false);
	API_DTO_FIELD_REQUIRE(String, itemTypeCode, ZH_WORDS_GETTER("treatment.field.item-type-code"), false);
	DTO_FIELD(String, itemTypeCodeDictText, "itemTypeCode_dictText");
	DTO_FIELD_INFO(itemTypeCodeDictText)
	{
		info->description = ZH_WORDS_GETTER("treatment.field.item-type-code-text");
		info->required = false;
	}
	API_DTO_FIELD_REQUIRE(String, typeCode, ZH_WORDS_GETTER("treatment.field.type-code"), false);
	API_DTO_FIELD_REQUIRE(String, packageUnitCode, ZH_WORDS_GETTER("treatment.field.package-unit-code"), false);
	DTO_FIELD(String, packageUnitCodeDictText, "packageUnitCode_dictText");
	DTO_FIELD_INFO(packageUnitCodeDictText)
	{
		info->description = ZH_WORDS_GETTER("treatment.field.package-unit-code-text");
		info->required = false;
	}
	API_DTO_FIELD_REQUIRE(String, defEncounterUnitCode, ZH_WORDS_GETTER("treatment.field.def-encounter-unit-code"), false);
	DTO_FIELD(String, defEncounterUnitCodeDictText, "defEncounterUnitCode_dictText");
	DTO_FIELD_INFO(defEncounterUnitCodeDictText)
	{
		info->description = ZH_WORDS_GETTER("treatment.field.def-encounter-unit-code-text");
		info->required = false;
	}
	API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("treatment.field.yb-no"), false);
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("treatment.field.status"), true);
	API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("treatment.field.version"), true);
};

/** 新版诊疗选择列表分页数据。 */
class TreatmentOptionsPageDTO : public PageDTO<TreatmentOptionV2DTO::Wrapper>
{
	DTO_INIT(TreatmentOptionsPageDTO, PageDTO<TreatmentOptionV2DTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENT_OPTIONS_DTO_H_
