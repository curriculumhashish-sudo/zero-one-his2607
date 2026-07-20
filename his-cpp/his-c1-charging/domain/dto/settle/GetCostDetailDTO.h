#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: ovo
 @Date: 2026/07/20

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
#ifndef _GET_COST_DETAIL_DTO_H_
#define _GET_COST_DETAIL_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostDetailItemDTO : public oatpp::DTO
{
	DTO_INIT(CostDetailItemDTO, DTO);

	API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("charging.settle.field.chargeItemId"), true, "329906419492982785");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("charging.settle.field.chargeBusNo"), true, "CI2026-06-290020");
	API_DTO_FIELD(String, prescriptionNo, ZH_WORDS_GETTER("charging.settle.field.prescriptionNo"), false, "RX202607200001");
	API_DTO_FIELD(String, itemName, ZH_WORDS_GETTER("charging.settle.field.itemName"), true, "Amoxicillin capsules");
	API_DTO_FIELD(String, chargeCategoryCode, ZH_WORDS_GETTER("charging.settle.field.chargeCategoryCode"), true, "01");
	API_DTO_FIELD(String, chargeCategoryName, ZH_WORDS_GETTER("charging.settle.field.chargeCategoryName"), true, "Medicine fee");
	API_DTO_FIELD(Float64, quantityValue, ZH_WORDS_GETTER("charging.settle.field.quantityValue"), true, 2.0);
	API_DTO_FIELD(String, quantityUnit, ZH_WORDS_GETTER("charging.settle.field.quantityUnit"), true, "box");
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("charging.settle.field.unitPrice"), true, 32.5);
	API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("charging.settle.field.totalPrice"), true, 65.0);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("charging.settle.field.chargeStatusEnum"), true, "UNSETTLED");
	API_DTO_FIELD(String, occurrenceTime, ZH_WORDS_GETTER("charging.settle.field.occurrenceTime"), true, "2026-07-20 10:30:00");
	API_DTO_FIELD(String, performingOrgName, ZH_WORDS_GETTER("charging.settle.field.performingOrgName"), false, "Inpatient Pharmacy");
	API_DTO_FIELD(String, entererName, ZH_WORDS_GETTER("charging.settle.field.entererName"), false, "Nurse Zhang");
};

class GetCostDetailDTO : public oatpp::DTO
{
	DTO_INIT(GetCostDetailDTO, DTO);

	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("charging.settle.field.patientId"), true, "329874660197928960");
	API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("charging.settle.field.patientName"), true, "Zhang San");
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("charging.settle.field.encounterId"), true, "329874660147597312");
	API_DTO_FIELD(String, encounterBusNo, ZH_WORDS_GETTER("charging.settle.field.encounterBusNo"), true, "IP202607200001");
	API_DTO_FIELD(Float64, totalAmount, ZH_WORDS_GETTER("charging.settle.field.totalAmount"), true, 1280.5);
	API_DTO_FIELD(Float64, settledAmount, ZH_WORDS_GETTER("charging.settle.field.settledAmount"), true, 800.0);
	API_DTO_FIELD(Float64, unsettledAmount, ZH_WORDS_GETTER("charging.settle.field.unsettledAmount"), true, 480.5);
	API_DTO_FIELD(Vector<Object<CostDetailItemDTO>>, items, ZH_WORDS_GETTER("charging.settle.field.costDetailItems"), true, {});

public:
	GetCostDetailDTO()
	{
		items = Vector<Object<CostDetailItemDTO>>::createShared();
		items->push_back(CostDetailItemDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_COST_DETAIL_DTO_H_
