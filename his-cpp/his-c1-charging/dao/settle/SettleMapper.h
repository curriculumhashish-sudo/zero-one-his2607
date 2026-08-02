#pragma once

#ifndef _SETTLE_MAPPER_H_
#define _SETTLE_MAPPER_H_

#include "Mapper.h"
#include "domain/do/settle/SettleDO.h"

// 结算账户映射器
class SettlementAccountMapper : public Mapper<PtrSettlementAccountDO>
{
public:
	PtrSettlementAccountDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<SettlementAccountDO>();
		data->setId(resultSet->getString("id"));
		data->setTenantId(resultSet->getString("tenant_id"));
		data->setUpdatedAt(resultSet->getString("updated_at"));
		data->setUpdatedBy(resultSet->getString("updated_by"));
		data->setIsDeleted(resultSet->getInt("is_deleted"));
		data->setVersion(resultSet->getInt("version"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setBusNo(resultSet->getString("bus_no"));
		data->setBalance(resultSet->getString("balance"));
		data->setBillingStatusEnum(resultSet->getString("billing_status_enum"));
		data->setContractNo(resultSet->getString("contract_no"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		return data;
	}
};

// 待结算收费项目映射器
class SettlementChargeItemMapper : public Mapper<PtrSettlementChargeItemDO>
{
public:
	PtrSettlementChargeItemDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<SettlementChargeItemDO>();
		data->setId(resultSet->getString("id"));
		data->setTenantId(resultSet->getString("tenant_id"));
		data->setUpdatedAt(resultSet->getString("updated_at"));
		data->setUpdatedBy(resultSet->getString("updated_by"));
		data->setIsDeleted(resultSet->getInt("is_deleted"));
		data->setVersion(resultSet->getInt("version"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setAccountId(resultSet->getString("account_id"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setTotalPrice(resultSet->getString("total_price"));
		return data;
	}
};

#endif
