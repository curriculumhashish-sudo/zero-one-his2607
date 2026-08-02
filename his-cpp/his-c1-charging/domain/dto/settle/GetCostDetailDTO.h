#pragma once

#ifndef _GET_COST_DETAIL_DTO_H_
#define _GET_COST_DETAIL_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostDetailItemDTO : public oatpp::DTO {
	DTO_INIT(CostDetailItemDTO, DTO);

	API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("ovo.charging.settle.charge_item_id"), true, "329906419492982785");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("ovo.charging.settle.charge_item_bus_no"), true, "CI2026-06-290020");
	API_DTO_FIELD(String, prescriptionNo, ZH_WORDS_GETTER("ovo.charging.settle.prescription_no"), false, "RX202607200001");
	API_DTO_FIELD(String, itemName, ZH_WORDS_GETTER("ovo.charging.settle.item_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.item_name_example"));
	API_DTO_FIELD(String, chargeCategoryCode, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_code"), true, "01");
	API_DTO_FIELD(String, chargeCategoryName, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_name_example"));
	API_DTO_FIELD(Float64, quantityValue, ZH_WORDS_GETTER("ovo.charging.settle.quantity_value"), true, 2.0);
	API_DTO_FIELD(String, quantityUnit, ZH_WORDS_GETTER("ovo.charging.settle.quantity_unit"), true, ZH_WORDS_GETTER("ovo.charging.settle.quantity_unit_example"));
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("ovo.charging.settle.unit_price"), true, 32.5);
	API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("ovo.charging.settle.total_price"), true, 65.0);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.charge_status"), true, "UNSETTLED");
	API_DTO_FIELD(String, occurrenceTime, ZH_WORDS_GETTER("ovo.charging.settle.occurrence_time"), true, "2026-07-20 10:30:00");
	API_DTO_FIELD(String, performingOrgName, ZH_WORDS_GETTER("ovo.charging.settle.performing_org_name"), false, ZH_WORDS_GETTER("ovo.charging.settle.performing_org_name_example"));
	API_DTO_FIELD(String, entererName, ZH_WORDS_GETTER("ovo.charging.settle.enterer_name"), false, ZH_WORDS_GETTER("ovo.charging.settle.enterer_name_example"));
};

class GetCostDetailDTO : public oatpp::DTO {
	DTO_INIT(GetCostDetailDTO, DTO);

	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.patient_id"), true, "329874660197928960");
	API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("ovo.charging.settle.patient_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.patient_name_example"));
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.encounter_id"), true, "329874660147597312");
	API_DTO_FIELD(String, encounterBusNo, ZH_WORDS_GETTER("ovo.charging.settle.encounter_bus_no"), true, "IP202607200001");
	API_DTO_FIELD(Float64, totalAmount, ZH_WORDS_GETTER("ovo.charging.settle.total_amount"), true, 1280.5);
	API_DTO_FIELD(Float64, settledAmount, ZH_WORDS_GETTER("ovo.charging.settle.settled_amount"), true, 800.0);
	API_DTO_FIELD(Float64, unsettledAmount, ZH_WORDS_GETTER("ovo.charging.settle.unsettled_amount"), true, 480.5);
	API_DTO_FIELD(Vector<Object<CostDetailItemDTO>>, items, ZH_WORDS_GETTER("ovo.charging.settle.cost_items"), true, {});

public:
	GetCostDetailDTO() {
		items = Vector<Object<CostDetailItemDTO>>::createShared();
		items->push_back(CostDetailItemDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif
