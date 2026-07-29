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
	API_DTO_FIELD(Vector<Object<SettlePaymentDetailDTO>>, paymentDetails, ZH_WORDS_GETTER("ovo.charging.settle.payment_details"), true, {});

public:
	SettleHistoryItemDTO() {
		paymentDetails = Vector<Object<SettlePaymentDetailDTO>>::createShared();
		paymentDetails->push_back(SettlePaymentDetailDTO::createShared());
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
