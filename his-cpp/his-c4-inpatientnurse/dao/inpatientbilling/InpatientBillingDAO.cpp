#include "stdafx.h"
#include "InpatientBillingDAO.h"
#include "Mapper.h"

// cus_charge_item_auto_gen 表结果映射
class AutoGenRuleMapper : public Mapper<AutoGenRuleDO>
{
public:
	AutoGenRuleDO mapper(ResultSet* resultSet) const override
	{
		AutoGenRuleDO data;
		data.setId(resultSet->getString(1));
		data.setTenantId(resultSet->getString(2));
		data.setCreatedAt(resultSet->getString(3));
		data.setUpdatedAt(resultSet->getString(4));
		data.setCreatedBy(resultSet->getString(5));
		data.setUpdatedBy(resultSet->getString(6));
		data.setIsDeleted(resultSet->getBoolean(7));
		data.setDeletedAt(resultSet->getString(8));
		data.setDeletedBy(resultSet->getString(9));
		data.setVersion(resultSet->getInt(10));
		data.setRuleName(resultSet->getString(11));
		data.setStatusEnum(resultSet->getString(12));
		data.setTriggerSourceTable(resultSet->getString(13));
		data.setTriggerSourceId(resultSet->getString(14));
		data.setChargeDefinitionId(resultSet->getString(15));
		data.setPerformingOrgId(resultSet->getString(16));
		data.setQuantityValue(resultSet->getDouble(17));
		data.setPriority(resultSet->getInt(18));
		data.setRemark(resultSet->getString(19));
		data.setEncounterId(resultSet->getString(20));
		return data;
	}
};

std::list<AutoGenRuleDO> InpatientBillingDAO::selectRulesByEncounterId(const std::string& encounterId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	std::string sql =
		"SELECT id, tenant_id, created_at, updated_at, created_by, updated_by, "
		"is_deleted, deleted_at, deleted_by, version, "
		"rule_name, status_enum, trigger_source_table, trigger_source_id, "
		"charge_definition_id, performing_org_id, quantity_value, "
		"priority, remark, encounter_id "
		"FROM cus_charge_item_auto_gen "
		"WHERE is_deleted = 0 AND encounter_id = ? "
		"ORDER BY priority ASC, id ASC";
	return sqlSession->executeQuery<AutoGenRuleDO>(sql, AutoGenRuleMapper(), params);
}

uint64_t InpatientBillingDAO::countRulesByEncounterId(const std::string& encounterId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	std::string sql =
		"SELECT COUNT(*) FROM cus_charge_item_auto_gen "
		"WHERE is_deleted = 0 AND encounter_id = ?";
	return sqlSession->executeQueryNumerical(sql, params);
}
