#pragma once

#ifndef _GET_SETTLE_HISTORY_MAPPER_H_
#define _GET_SETTLE_HISTORY_MAPPER_H_

#include "Mapper.h"
#include "domain/do/settle/GetSettleHistoryDO.h"

// 患者历史结算记录映射器
class GetSettleHistoryMapper : public Mapper<PtrGetSettleHistoryDO>
{
public:
	PtrGetSettleHistoryDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<GetSettleHistoryDO>();
		data->setSettlementId(resultSet->getString("settlement_id"));
		data->setPaymentNo(resultSet->getString("payment_no"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setPatientName(resultSet->getString("patient_name"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setEncounterBusNo(resultSet->getString("encounter_bus_no"));
		data->setKindEnum(resultSet->getString("kind_enum"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setDisplayAmount(resultSet->getString("display_amount"));
		data->setTenderedAmount(resultSet->getString("tendered_amount"));
		data->setReturnedAmount(resultSet->getString("returned_amount"));
		data->setBillDate(resultSet->getString("bill_date"));
		data->setInvoiceId(resultSet->getString("invoice_id"));
		data->setSetlId(resultSet->getString("setl_id"));
		return data;
	}
};

// 历史结算支付明细映射器
class SettleHistoryPaymentDetailMapper :
	public Mapper<PtrSettleHistoryPaymentDetailDO>
{
public:
	PtrSettleHistoryPaymentDetailDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<SettleHistoryPaymentDetailDO>();
		data->setPaymentDetailId(resultSet->getString("payment_detail_id"));
		data->setReconciliationId(resultSet->getString("reconciliation_id"));
		data->setPayEnum(resultSet->getString("pay_enum"));
		data->setAmount(resultSet->getString("amount"));
		data->setReturnAmount(resultSet->getString("return_amount"));
		data->setPayTransNo(resultSet->getString("pay_trans_no"));
		data->setPayTransDate(resultSet->getString("pay_trans_date"));
		data->setResultEnum(resultSet->getString("result_enum"));
		return data;
	}
};

#endif
