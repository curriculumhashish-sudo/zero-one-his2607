#pragma once

#ifndef _GET_SETTLE_HISTORY_DO_H_
#define _GET_SETTLE_HISTORY_DO_H_

#include "../DoInclude.h"

// 单条住院结算或退结算记录只读投影
// 数据由 fin_payment_reconciliation、adm_patient、adm_encounter 和 fin_payment_yb_settlement 联合查询得到
class GetSettleHistoryDO : public BaseDO {
	MYSQL_SYNTHESIZE(std::string, settlementId, SettlementId);
	MYSQL_SYNTHESIZE(std::string, paymentNo, PaymentNo);
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, patientName, PatientName);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, encounterBusNo, EncounterBusNo);
	MYSQL_SYNTHESIZE(std::string, kindEnum, KindEnum);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(std::string, displayAmount, DisplayAmount);
	MYSQL_SYNTHESIZE(std::string, tenderedAmount, TenderedAmount);
	MYSQL_SYNTHESIZE(std::string, returnedAmount, ReturnedAmount);
	MYSQL_SYNTHESIZE(std::string, billDate, BillDate);
	MYSQL_SYNTHESIZE(std::string, invoiceId, InvoiceId);
	MYSQL_SYNTHESIZE(std::string, setlId, SetlId);

public:
	GetSettleHistoryDO() : BaseDO("") {
		MYSQL_ADD_FIELD("settlement_id", "s", settlementId);
		MYSQL_ADD_FIELD("payment_no", "s", paymentNo);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("patient_name", "s", patientName);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("encounter_bus_no", "s", encounterBusNo);
		MYSQL_ADD_FIELD("kind_enum", "s", kindEnum);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("display_amount", "s", displayAmount);
		MYSQL_ADD_FIELD("tendered_amount", "s", tenderedAmount);
		MYSQL_ADD_FIELD("returned_amount", "s", returnedAmount);
		MYSQL_ADD_FIELD("bill_date", "s", billDate);
		MYSQL_ADD_FIELD("invoice_id", "s", invoiceId);
		MYSQL_ADD_FIELD("setl_id", "s", setlId);
	}
};

typedef std::shared_ptr<GetSettleHistoryDO> PtrGetSettleHistoryDO;

// 单条结算支付渠道明细只读投影
class SettleHistoryPaymentDetailDO : public BaseDO {
	MYSQL_SYNTHESIZE(std::string, paymentDetailId, PaymentDetailId);
	MYSQL_SYNTHESIZE(std::string, reconciliationId, ReconciliationId);
	MYSQL_SYNTHESIZE(std::string, payEnum, PayEnum);
	MYSQL_SYNTHESIZE(std::string, amount, Amount);
	MYSQL_SYNTHESIZE(std::string, returnAmount, ReturnAmount);
	MYSQL_SYNTHESIZE(std::string, payTransNo, PayTransNo);
	MYSQL_SYNTHESIZE(std::string, payTransDate, PayTransDate);
	MYSQL_SYNTHESIZE(std::string, resultEnum, ResultEnum);

public:
	SettleHistoryPaymentDetailDO() : BaseDO("") {
		MYSQL_ADD_FIELD("payment_detail_id", "s", paymentDetailId);
		MYSQL_ADD_FIELD("reconciliation_id", "s", reconciliationId);
		MYSQL_ADD_FIELD("pay_enum", "s", payEnum);
		MYSQL_ADD_FIELD("amount", "s", amount);
		MYSQL_ADD_FIELD("return_amount", "s", returnAmount);
		MYSQL_ADD_FIELD("pay_trans_no", "s", payTransNo);
		MYSQL_ADD_FIELD("pay_trans_date", "s", payTransDate);
		MYSQL_ADD_FIELD("result_enum", "s", resultEnum);
	}
};

typedef std::shared_ptr<SettleHistoryPaymentDetailDO> PtrSettleHistoryPaymentDetailDO;

#endif
