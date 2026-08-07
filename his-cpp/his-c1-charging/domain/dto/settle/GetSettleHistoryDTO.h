#pragma once

#ifndef _GET_SETTLE_HISTORY_DTO_H_
#define _GET_SETTLE_HISTORY_DTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettlePaymentDetailDTO : public oatpp::DTO {
	DTO_INIT(SettlePaymentDetailDTO, DTO);

	API_DTO_FIELD(String, paymentDetailId, ZH_WORDS_GETTER("ovo.charging.settle.payment_detail_id"), true, "330100000000000101");
	API_DTO_FIELD(String, payEnum, ZH_WORDS_GETTER("ovo.charging.settle.pay_enum"), true, "CASH");
	API_DTO_FIELD(Float64, amount, ZH_WORDS_GETTER("ovo.charging.settle.payment_amount"), true, 480.5);
	API_DTO_FIELD(Float64, returnAmount, ZH_WORDS_GETTER("ovo.charging.settle.return_amount"), false, 0.0);
	API_DTO_FIELD(String, payTransNo, ZH_WORDS_GETTER("ovo.charging.settle.pay_trans_no"), false, "CASH202607200001");
	API_DTO_FIELD(String, payTransDate, ZH_WORDS_GETTER("ovo.charging.settle.pay_trans_date"), true, "2026-07-20 11:00:00");
	API_DTO_FIELD(String, resultEnum, ZH_WORDS_GETTER("ovo.charging.settle.pay_result"), true, "SUCCESS");
};

// 单条结算记录的费用分类汇总
class SettleHistoryCategoryDTO : public oatpp::DTO {
	DTO_INIT(SettleHistoryCategoryDTO, DTO);

	API_DTO_FIELD(String, categoryCode, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_code"), true, "01");
	API_DTO_FIELD(String, categoryName, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_name_example"));
	API_DTO_FIELD(Int32, lineCount, ZH_WORDS_GETTER("ovo.charging.settle.category_line_count"), true, 1);
	API_DTO_FIELD(Float64, totalAmount, ZH_WORDS_GETTER("ovo.charging.settle.category_total_amount"), true, 65.0);
};

// 单条结算记录的费用明细行
class SettleHistoryLineDTO : public oatpp::DTO {
	DTO_INIT(SettleHistoryLineDTO, DTO);

	API_DTO_FIELD(String, chargeItemId, ZH_WORDS_GETTER("ovo.charging.settle.charge_item_id"), true, "329906419492982785");
	API_DTO_FIELD(String, itemName, ZH_WORDS_GETTER("ovo.charging.settle.item_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.item_name_example"));
	API_DTO_FIELD(String, chargeCategoryCode, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_code"), true, "01");
	API_DTO_FIELD(String, chargeCategoryName, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.charge_category_name_example"));
	API_DTO_FIELD(Float64, quantityValue, ZH_WORDS_GETTER("ovo.charging.settle.quantity_value"), true, 2.0);
	API_DTO_FIELD(String, quantityUnit, ZH_WORDS_GETTER("ovo.charging.settle.quantity_unit"), true, ZH_WORDS_GETTER("ovo.charging.settle.quantity_unit_example"));
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("ovo.charging.settle.unit_price"), true, 32.5);
	API_DTO_FIELD(Float64, totalPrice, ZH_WORDS_GETTER("ovo.charging.settle.total_price"), true, 65.0);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.charge_status"), true, "SETTLED");
	API_DTO_FIELD(String, occurrenceTime, ZH_WORDS_GETTER("ovo.charging.settle.occurrence_time"), true, "2026-07-20 10:30:00");
};

class SettleHistoryItemDTO : public oatpp::DTO {
	DTO_INIT(SettleHistoryItemDTO, DTO);

	API_DTO_FIELD(String, settlementId, ZH_WORDS_GETTER("ovo.charging.settle.settlement_id"), true, "330100000000000001");
	API_DTO_FIELD(String, paymentNo, ZH_WORDS_GETTER("ovo.charging.settle.payment_no"), true, "IPAY202607200001");
	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.patient_id"), true, "329874660197928960");
	API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("ovo.charging.settle.patient_name"), true, ZH_WORDS_GETTER("ovo.charging.settle.patient_name_example"));
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.encounter_id"), true, "329874660147597312");
	API_DTO_FIELD(String, encounterBusNo, ZH_WORDS_GETTER("ovo.charging.settle.encounter_bus_no"), true, "IP202607200001");
	API_DTO_FIELD(String, kindEnum, ZH_WORDS_GETTER("ovo.charging.settle.kind_enum"), true, "SETTLEMENT");
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.settlement_status"), true, "CONFIRMED");
	API_DTO_FIELD(Float64, displayAmount, ZH_WORDS_GETTER("ovo.charging.settle.display_amount"), true, 480.5);
	API_DTO_FIELD(Float64, tenderedAmount, ZH_WORDS_GETTER("ovo.charging.settle.tendered_amount"), true, 500.0);
	API_DTO_FIELD(Float64, returnedAmount, ZH_WORDS_GETTER("ovo.charging.settle.returned_amount"), true, 19.5);
	API_DTO_FIELD(String, billDate, ZH_WORDS_GETTER("ovo.charging.settle.bill_date"), true, "2026-07-20 11:00:00");
	API_DTO_FIELD(String, invoiceId, ZH_WORDS_GETTER("ovo.charging.settle.invoice_id"), false, "330100000000000201");
	API_DTO_FIELD(String, setlId, ZH_WORDS_GETTER("ovo.charging.settle.setl_id"), false, "SETL202607200001");
	API_DTO_FIELD(String, activePaymentId, ZH_WORDS_GETTER("ovo.charging.settle.active_payment_id"), true, "330100000000000001");
	API_DTO_FIELD(Float64, receivableTotal, ZH_WORDS_GETTER("ovo.charging.settle.receivable_total"), true, 480.5);
	API_DTO_FIELD(Float64, netSettleTotal, ZH_WORDS_GETTER("ovo.charging.settle.net_settle_total"), true, 480.5);
	API_DTO_FIELD(String, settleTime, ZH_WORDS_GETTER("ovo.charging.settle.settle_time"), true, "2026-07-20 11:00:00");
	API_DTO_FIELD(Vector<Object<SettleHistoryCategoryDTO>>, categories, ZH_WORDS_GETTER("ovo.charging.settle.history_categories"), true, {});
	API_DTO_FIELD(Vector<Object<SettleHistoryLineDTO>>, lines, ZH_WORDS_GETTER("ovo.charging.settle.history_lines"), true, {});
	API_DTO_FIELD(Vector<Object<SettlePaymentDetailDTO>>, payments, ZH_WORDS_GETTER("ovo.charging.settle.history_payments"), true, {});

public:
	SettleHistoryItemDTO() {
		categories = Vector<Object<SettleHistoryCategoryDTO>>::createShared();
		categories->push_back(SettleHistoryCategoryDTO::createShared());
		lines = Vector<Object<SettleHistoryLineDTO>>::createShared();
		lines->push_back(SettleHistoryLineDTO::createShared());
		payments = Vector<Object<SettlePaymentDetailDTO>>::createShared();
		payments->push_back(SettlePaymentDetailDTO::createShared());
	}
};

class GetSettleHistoryPageDTO : public PageDTO<SettleHistoryItemDTO::Wrapper> {
	DTO_INIT(GetSettleHistoryPageDTO, PageDTO<SettleHistoryItemDTO::Wrapper>);

public:
	GetSettleHistoryPageDTO() {
		pageIndex = 1;
		pageSize = 10;
		total = 1;
		pages = 1;
		rows = List<SettleHistoryItemDTO::Wrapper>::createShared();
		rows->push_back(SettleHistoryItemDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif
