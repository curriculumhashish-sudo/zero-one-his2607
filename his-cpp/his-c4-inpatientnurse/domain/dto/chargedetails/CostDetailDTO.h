#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
#ifndef _COSTDETAILDTO_H_
#define _COSTDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostDetailDTO : public oatpp::DTO {
	DTO_INIT(CostDetailDTO, DTO);

	// id
	API_DTO_FIELD(String, id, ZH_WORDS_GETTER("chargedetails.field.id"), true, "");
	// patient id
	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("chargedetails.field.patientId"), true, "");
	// encounter id
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("chargedetails.field.encounterId"), true, "");
	// charge name (from definition)
	API_DTO_FIELD(String, chargeName, ZH_WORDS_GETTER("chargedetails.field.chargeName"), true, "");
	// type code
	API_DTO_FIELD(String, typeCode, ZH_WORDS_GETTER("chargedetails.field.typeCode"), true, "");
	// unit price
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("chargedetails.field.unitPrice"), true, 0.0);
	// quantity value
	API_DTO_FIELD(Float64, quantityValue, ZH_WORDS_GETTER("chargedetails.field.quantityValue"), true, 0.0);
	// quantity unit
	API_DTO_FIELD(String, quantityUnit, ZH_WORDS_GETTER("chargedetails.field.quantityUnit"), true, "");
	// total price
	API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("chargedetails.field.totalPrice"), true, 0.0);
	// base amount
	API_DTO_FIELD(Float64, baseAmount, ZH_WORDS_GETTER("chargedetails.field.baseAmount"), true, 0.0);
	// discount amount
	API_DTO_FIELD(Float64, discountAmount, ZH_WORDS_GETTER("chargedetails.field.discountAmount"), true, 0.0);
	// occurrence time
	API_DTO_FIELD(String, occurrenceTime, ZH_WORDS_GETTER("chargedetails.field.occurrenceTime"), true, "");
	// performing org id
	API_DTO_FIELD(String, performingOrgId, ZH_WORDS_GETTER("chargedetails.field.performingOrgId"), true, "");
	// status enum
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("chargedetails.field.statusEnum"), true, "");
	// bus no
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("chargedetails.field.busNo"), true, "");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTDETAILDTO_H_
