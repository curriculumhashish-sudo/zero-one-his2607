#pragma once

#ifndef _GET_COST_DETAIL_DTO_H_
#define _GET_COST_DETAIL_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostDetailItemDTO : public oatpp::DTO
{
	DTO_INIT(CostDetailItemDTO, DTO);

	API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.chargeItemId.description"), true, "329906419492982785");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.busNo.description"), true, "CI2026-06-290020");
	API_DTO_FIELD(String, prescriptionNo, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.prescriptionNo.description"), false, "RX202607200001");
	API_DTO_FIELD(String, itemName, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.itemName.description"), true, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.itemName.value"));
	API_DTO_FIELD(String, chargeCategoryCode, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.chargeCategoryCode.description"), true, "01");
	API_DTO_FIELD(String, chargeCategoryName, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.chargeCategoryName.description"), true, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.chargeCategoryName.value"));
	API_DTO_FIELD(Float64, quantityValue, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.quantityValue.description"), true, 2.0);
	API_DTO_FIELD(String, quantityUnit, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.quantityUnit.description"), true, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.quantityUnit.value"));
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.unitPrice.description"), true, 32.5);
	API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.totalPrice.description"), true, 65.0);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.statusEnum.description"), true, "UNSETTLED");
	API_DTO_FIELD(String, occurrenceTime, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.occurrenceTime.description"), true, "2026-07-20 10:30:00");
	API_DTO_FIELD(String, performingOrgName, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.performingOrgName.description"), false, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.performingOrgName.value"));
	API_DTO_FIELD(String, entererName, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.entererName.description"), false, ZH_WORDS_GETTER("ovo.charging.settle.CostDetailItemDTO.entererName.value"));
};

class GetCostDetailDTO : public oatpp::DTO
{
	DTO_INIT(GetCostDetailDTO, DTO);

	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.patientId.description"), true, "329874660197928960");
	API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.patientName.description"), true, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.patientName.value"));
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.encounterId.description"), true, "329874660147597312");
	API_DTO_FIELD(String, encounterBusNo, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.encounterBusNo.description"), true, "IP202607200001");
	API_DTO_FIELD(Float64, totalAmount, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.totalAmount.description"), true, 1280.5);
	API_DTO_FIELD(Float64, settledAmount, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.settledAmount.description"), true, 800.0);
	API_DTO_FIELD(Float64, unsettledAmount, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.unsettledAmount.description"), true, 480.5);
	API_DTO_FIELD(Vector<Object<CostDetailItemDTO>>, items, ZH_WORDS_GETTER("ovo.charging.settle.GetCostDetailDTO.items.description"), true, {});

public:
	GetCostDetailDTO()
	{
		items = Vector<Object<CostDetailItemDTO>>::createShared();
		items->push_back(CostDetailItemDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_COST_DETAIL_DTO_H_
