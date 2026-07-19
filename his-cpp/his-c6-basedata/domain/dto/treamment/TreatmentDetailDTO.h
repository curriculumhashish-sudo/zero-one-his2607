#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/19 00:00:00

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
#ifndef _TREATMENTDETAILDTO_H_
#define _TREATMENTDETAILDTO_H_

#include "../../GlobalInclude.h"
#include "TreatmentChildDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Treatment item detail DTO.
 * Returned by "query-one" so the front-end can render the read-only detail view
 * or pre-fill the edit form. Defined standalone (does not inherit a TreatmentAddDTO)
 * because no add/modify DTO exists in this module yet.
 */
class TreatmentDetailDTO : public oatpp::DTO
{
	DTO_INIT(TreatmentDetailDTO, DTO);

	// Treatment item primary key
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
	// Treatment item business code (list column, not shown on the edit form)
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("treatment.field.code"), true);
	// Enabled/disabled status (list column, not shown on the edit form): 0-disabled 1-enabled
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("treatment.field.status"), true);

	// Basic information
	// Treatment item name
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
	// Category code (dictionary code only, no display text)
	API_DTO_FIELD_REQUIRE(String, categoryCode, ZH_WORDS_GETTER("treatment.field.category-code"), true);
	// Business type enum: 1-examination 2-inspection 3-nursing 4-surgery 5-other
	API_DTO_FIELD_REQUIRE(Int32, typeEnum, ZH_WORDS_GETTER("treatment.field.type-enum"), true);
	// Sale/pricing unit code
	API_DTO_FIELD_REQUIRE(String, packageUnitCode, ZH_WORDS_GETTER("treatment.field.package-unit-code"), false);
	// Pinyin quick code
	API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("treatment.field.py-str"), false);
	// Wubi quick code
	API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("treatment.field.wb-str"), false);
	// Finance type code
	API_DTO_FIELD_REQUIRE(String, itemTypeCode, ZH_WORDS_GETTER("treatment.field.item-type-code"), false);
	// Purchase/cost price
	API_DTO_FIELD_REQUIRE(Float64, purchasePrice, ZH_WORDS_GETTER("treatment.field.purchase-price"), false);
	// Retail price
	API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("treatment.field.retail-price"), false);
	// Maximum retail price
	API_DTO_FIELD_REQUIRE(Float64, maximumRetailPrice, ZH_WORDS_GETTER("treatment.field.maximum-retail-price"), false);
	// Description text
	API_DTO_FIELD_REQUIRE(String, descriptionText, ZH_WORDS_GETTER("treatment.field.description-text"), false);

	// Medical insurance information
	// Medical insurance flag: 0-no 1-yes
	API_DTO_FIELD_REQUIRE(Int32, ybFlag, ZH_WORDS_GETTER("treatment.field.yb-flag"), false);
	// Medical insurance registration code
	API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("treatment.field.yb-no"), false);
	// Medical insurance category code
	API_DTO_FIELD_REQUIRE(String, ybType, ZH_WORDS_GETTER("treatment.field.yb-type"), false);
	// Medical insurance code-matching flag: 0-unmatched 1-matched
	API_DTO_FIELD_REQUIRE(Int32, ybMatchFlag, ZH_WORDS_GETTER("treatment.field.yb-match-flag"), false);
	// Medical insurance charge item level: 1-class A 2-class B 3-class C
	API_DTO_FIELD_REQUIRE(Int32, chrgitmLv, ZH_WORDS_GETTER("treatment.field.chrgitm-lv"), false);

	// Sub-items (combo/package treatment lines)
	API_DTO_FIELD_REQUIRE(List<TreatmentChildDTO::Wrapper>, children, ZH_WORDS_GETTER("treatment.field.children"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTDETAILDTO_H_
