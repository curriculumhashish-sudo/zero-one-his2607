#include "stdafx.h"
#include "SettleDAO.h"
#include "SettleMapper.h"

PtrSettlementAccountDO SettleDAO::selectAccountForUpdate(
	const std::string& patientId,
	const std::string& encounterId,
	const std::string& accountId)
{
	const std::string sql =
		"SELECT id,tenant_id,updated_at,updated_by,is_deleted,version,"
		" patient_id,encounter_id,bus_no,balance,billing_status_enum,"
		" contract_no,status_enum"
		" FROM adm_account"
		" WHERE id=?"
		" AND patient_id=?"
		" AND encounter_id=?"
		" AND is_deleted=0"
		" FOR UPDATE";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, accountId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	return sqlSession->executeQueryOne<PtrSettlementAccountDO>(sql, SettlementAccountMapper(), params);
}

std::list<PtrSettlementChargeItemDO> SettleDAO::selectChargeItemsForUpdate(
	const std::string& patientId,
	const std::string& encounterId,
	const std::string& accountId,
	const std::vector<std::string>& chargeItemIds)
{
	if (chargeItemIds.empty())
	{
		return {};
	}

	std::stringstream sql;
	SqlParams params;
	sql << "SELECT id,tenant_id,updated_at,updated_by,is_deleted,version,"
		<< " patient_id,encounter_id,account_id,status_enum,total_price"
		<< " FROM adm_charge_item"
		<< " WHERE patient_id=?"
		<< " AND encounter_id=?"
		<< " AND account_id=?"
		<< " AND is_deleted=0"
		<< " AND id IN (";
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, accountId);

	for (size_t index = 0; index < chargeItemIds.size(); ++index)
	{
		if (index > 0)
		{
			sql << ",";
		}
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, chargeItemIds[index]);
	}
	sql << ") FOR UPDATE";
	return sqlSession->executeQuery<PtrSettlementChargeItemDO>(sql.str(), SettlementChargeItemMapper(), params);
}

int SettleDAO::insertPayment(const SettlementPaymentDO& data)
{
	return insert(data);
}

int SettleDAO::insertPaymentDetails(const std::list<SettlementPaymentDetailDO>& dataList)
{
	int affectedRows = 0;
	for (const auto& data : dataList)
	{
		affectedRows += insert(data);
	}
	return affectedRows;
}

int SettleDAO::insertInvoice(const SettlementInvoiceDO& data)
{
	return insert(data);
}

int SettleDAO::insertPreSettle(const InpatientPreSettleDO& data)
{
	return insert(data);
}

int SettleDAO::insertYbSettlementRecord(const InpatientYbSettlementRecordDO& data)
{
	return insert(data);
}

int SettleDAO::insertYbLink(const SettlementYbLinkDO& data)
{
	return insert(data);
}

int SettleDAO::updateChargeItemsToSettled(
	const std::string& patientId,
	const std::string& encounterId,
	const std::string& accountId,
	const std::vector<std::string>& chargeItemIds,
	const std::string& updatedAt,
	const std::string& updatedBy)
{
	if (chargeItemIds.empty())
	{
		return 0;
	}

	std::stringstream sql;
	SqlParams params;
	sql << "UPDATE adm_charge_item"
		<< " SET status_enum='SETTLED',"
		<< " updated_at=?,"
		<< " updated_by=?,"
		<< " version=version+1"
		<< " WHERE patient_id=?"
		<< " AND encounter_id=?"
		<< " AND account_id=?"
		<< " AND is_deleted=0"
		<< " AND status_enum IN ('DRAFT','UNSETTLED')"
		<< " AND id IN (";
	SQLPARAMS_PUSH(params, "dt", std::string, updatedAt);
	SQLPARAMS_PUSH(params, "s", std::string, updatedBy);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, accountId);

	for (size_t index = 0; index < chargeItemIds.size(); ++index)
	{
		if (index > 0)
		{
			sql << ",";
		}
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, chargeItemIds[index]);
	}
	sql << ")";
	return sqlSession->executeUpdate(sql.str(), params);
}

int SettleDAO::updateAccountBillingStatus(
	const std::string& accountId,
	const std::string& patientId,
	const std::string& encounterId,
	const std::string& billingStatusEnum,
	const std::string& updatedAt,
	const std::string& updatedBy,
	int expectedVersion)
{
	const std::string sql =
		"UPDATE adm_account"
		" SET billing_status_enum=?,"
		" updated_at=?,"
		" updated_by=?,"
		" version=version+1"
		" WHERE id=?"
		" AND patient_id=?"
		" AND encounter_id=?"
		" AND version=?"
		" AND is_deleted=0";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, billingStatusEnum);
	SQLPARAMS_PUSH(params, "dt", std::string, updatedAt);
	SQLPARAMS_PUSH(params, "s", std::string, updatedBy);
	SQLPARAMS_PUSH(params, "s", std::string, accountId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "i", int, expectedVersion);
	return sqlSession->executeUpdate(sql, params);
}
