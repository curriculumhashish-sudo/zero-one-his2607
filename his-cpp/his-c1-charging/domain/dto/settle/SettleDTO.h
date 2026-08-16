#pragma once

#ifndef _SETTLE_DTO_H_
#define _SETTLE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettlePaymentDTO : public oatpp::DTO {
	DTO_INIT(SettlePaymentDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, payEnum, ZH_WORDS_GETTER("ovo.charging.settle.pay_enum"), true);
	API_DTO_FIELD_REQUIRE(Float64, amount, ZH_WORDS_GETTER("ovo.charging.settle.payment_amount"), true);
	API_DTO_FIELD_DEFAULT(String, payTransNo, ZH_WORDS_GETTER("ovo.charging.settle.pay_trans_no"));
};

// 扫码支付回执，仅扫码支付时传入
// 类名带Settle前缀，避免与门诊收费模块的ClientScanResultDTO冲突
class SettleClientScanResultDTO : public oatpp::DTO {
	DTO_INIT(SettleClientScanResultDTO, DTO);

	// 预留字段，暂不展开
};

class SettleRequestDTO : public oatpp::DTO {
	DTO_INIT(SettleRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.patient_id"), true);
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.encounter_id"), true);
	API_DTO_FIELD_REQUIRE(String, accountId, ZH_WORDS_GETTER("ovo.charging.settle.account_id"), true);
	API_DTO_FIELD_REQUIRE(Vector<String>, chargeItemIds, ZH_WORDS_GETTER("ovo.charging.settle.charge_item_ids"), true);
	API_DTO_FIELD_REQUIRE(Vector<Object<SettlePaymentDTO>>, paymentDetails, ZH_WORDS_GETTER("ovo.charging.settle.payment_details"), true);
	API_DTO_FIELD_REQUIRE(Float64, displayAmount, ZH_WORDS_GETTER("ovo.charging.settle.display_amount"), true);
	API_DTO_FIELD_DEFAULT(String, contractNo, ZH_WORDS_GETTER("ovo.charging.settle.contract_no"));
	API_DTO_FIELD_REQUIRE(String, billDate, ZH_WORDS_GETTER("ovo.charging.settle.bill_date"), true);
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("ovo.charging.settle.remark"));
	API_DTO_FIELD_DEFAULT(String, paymentDraftId, ZH_WORDS_GETTER("ovo.charging.settle.payment_draft_id"));
	API_DTO_FIELD_DEFAULT(String, ybPreSettleId, ZH_WORDS_GETTER("ovo.charging.settle.yb_pre_settle_id"));
	API_DTO_FIELD_DEFAULT(String, ybChrgBchno, ZH_WORDS_GETTER("ovo.charging.settle.yb_chrg_bchno"));
	API_DTO_FIELD_DEFAULT(Object<SettleClientScanResultDTO>, clientScanResult, ZH_WORDS_GETTER("ovo.charging.settle.client_scan_result"));

public:
	SettleRequestDTO() {
		chargeItemIds = Vector<String>::createShared();
		paymentDetails = Vector<Object<SettlePaymentDTO>>::createShared();
	}
};

class SettleResultDTO : public oatpp::DTO {
	DTO_INIT(SettleResultDTO, DTO);

	API_DTO_FIELD(String, paymentId, ZH_WORDS_GETTER("ovo.charging.settle.payment_id"), true, "330100000000000001");
	API_DTO_FIELD(String, paymentNo, ZH_WORDS_GETTER("ovo.charging.settle.payment_no"), true, "IPAY202607200001");
	API_DTO_FIELD_DEFAULT(String, preSettleId, ZH_WORDS_GETTER("ovo.charging.settle.pre_settle_id"));
	API_DTO_FIELD_DEFAULT(String, setlId, ZH_WORDS_GETTER("ovo.charging.settle.setl_id"));
	API_DTO_FIELD_DEFAULT(String, invoiceId, ZH_WORDS_GETTER("ovo.charging.settle.invoice_id"));
	API_DTO_FIELD(Float64, displayAmount, ZH_WORDS_GETTER("ovo.charging.settle.display_amount"), true, 480.5);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.settle_result_status"), true, "CONFIRMED");
	API_DTO_FIELD(String, settledAt, ZH_WORDS_GETTER("ovo.charging.settle.settled_at"), true, "2026-07-20 11:00:01");
};

#include OATPP_CODEGEN_END(DTO)

#endif
