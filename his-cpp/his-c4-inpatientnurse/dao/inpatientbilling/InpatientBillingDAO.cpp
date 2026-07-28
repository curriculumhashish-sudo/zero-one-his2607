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

// adm_charge_item 表结果映射
class ChargeItemSimpleMapper : public Mapper<ChargeItemDO>
{
public:
	ChargeItemDO mapper(ResultSet* resultSet) const override
	{
		ChargeItemDO data;
		data.setId(resultSet->getString(1));
		data.setProductId(resultSet->getString(2));
		data.setDefinitionId(resultSet->getString(3));
		data.setUnitPrice(resultSet->getDouble(4));
		data.setQuantityUnit(resultSet->getString(5));
		data.setStatusEnum(resultSet->getString(6));
		data.setTotalPrice(resultSet->getDouble(7));
		data.setPatientId(resultSet->getString(8));
		data.setEncounterId(resultSet->getString(9));
		data.setServiceId(resultSet->getString(10));
		data.setGenerateSourceEnum(resultSet->getString(11));
		data.setOccurrenceTime(resultSet->getString(12));
		data.setPerformingOrgId(resultSet->getString(13));
		data.setCostOrgId(resultSet->getString(14));
		data.setQuantityValue(resultSet->getDouble(15));
		return data;
	}
};

// 根据项目编码查询收费项目信息（补费时查询用）
std::unique_ptr<ChargeItemDO> InpatientBillingDAO::selectChargeItemByCode(const std::string& itemCode)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, itemCode);
	std::string sql =
		"SELECT id, product_id, definition_id, unit_price, quantity_unit, "
		"status_enum, total_price, patient_id, encounter_id, service_id, "
		"generate_source_enum, occurrence_time, performing_org_id, cost_org_id, quantity_value "
		"FROM adm_charge_item "
		"WHERE is_deleted = 0 AND product_id = ?";
	auto result = sqlSession->executeQuery<ChargeItemDO>(sql, ChargeItemSimpleMapper(), params);
	if (result.empty()) {
		return nullptr;
	}
	return std::make_unique<ChargeItemDO>(std::move(result.front()));
}

// 插入计费记录（补费）
int InpatientBillingDAO::insertChargeItem(const ChargeItemDO& chargeItem)
{
	std::string sql =
		"INSERT INTO adm_charge_item ("
		"id, product_id, definition_id, unit_price, quantity_unit, "
		"status_enum, total_price, patient_id, encounter_id, service_id, "
		"generate_source_enum, occurrence_time, performing_org_id, cost_org_id, quantity_value "
		") VALUES ("
		"?, ?, ?, ?, ?, "
		"?, ?, ?, ?, ?, "
		"?, ?, ?, ?, ?)";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getId());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getProductId());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getDefinitionId());
	SQLPARAMS_PUSH(params, "d", double, chargeItem.getUnitPrice());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getQuantityUnit());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getStatusEnum());
	SQLPARAMS_PUSH(params, "d", double, chargeItem.getTotalPrice());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getPatientId());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getEncounterId());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getServiceId());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getGenerateSourceEnum());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getOccurrenceTime());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getPerformingOrgId());
	SQLPARAMS_PUSH(params, "s", std::string, chargeItem.getCostOrgId());
	SQLPARAMS_PUSH(params, "d", double, chargeItem.getQuantityValue());

	return sqlSession->executeUpdate(sql, params);
}

// 根据患者ID和就诊ID查询费用明细
std::list<ChargeItemDO> InpatientBillingDAO::selectChargeItemsByPatientAndEncounter(
	const std::string& patientId,
	const std::string& encounterId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	std::string sql =
		"SELECT id, product_id, definition_id, unit_price, quantity_unit, "
		"status_enum, total_price, patient_id, encounter_id, service_id, "
		"generate_source_enum, occurrence_time, performing_org_id, cost_org_id, quantity_value "
		"FROM adm_charge_item "
		"WHERE is_deleted = 0 AND patient_id = ? AND encounter_id = ? "
		"ORDER BY created_at DESC";
	return sqlSession->executeQuery<ChargeItemDO>(sql, ChargeItemSimpleMapper(), params);
}

// 根据患者ID分页查询费用明细
std::list<ChargeItemDO> InpatientBillingDAO::selectChargeItemsByPatientId(
	const std::string& patientId,
	int offset,
	int limit)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "i", int, offset);
	SQLPARAMS_PUSH(params, "i", int, limit);
	std::string sql =
		"SELECT id, product_id, definition_id, unit_price, quantity_unit, "
		"status_enum, total_price, patient_id, encounter_id, service_id, "
		"generate_source_enum, occurrence_time, performing_org_id, cost_org_id, quantity_value "
		"FROM adm_charge_item "
		"WHERE is_deleted = 0 AND patient_id = ? "
		"ORDER BY created_at DESC "
		"LIMIT ? OFFSET ?";
	return sqlSession->executeQuery<ChargeItemDO>(sql, ChargeItemSimpleMapper(), params);
}

// 统计患者费用记录数量
uint64_t InpatientBillingDAO::countChargeItemsByPatientId(const std::string& patientId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	std::string sql =
		"SELECT COUNT(*) FROM adm_charge_item "
		"WHERE is_deleted = 0 AND patient_id = ?";
	return sqlSession->executeQueryNumerical(sql, params);
}

// 根据医嘱ID查询是否已补费
std::unique_ptr<ChargeItemDO> InpatientBillingDAO::selectChargeItemByServiceId(const std::string& serviceId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, serviceId);
	std::string sql =
		"SELECT id, product_id, definition_id, unit_price, quantity_unit, "
		"status_enum, total_price, patient_id, encounter_id, service_id, "
		"generate_source_enum, occurrence_time, performing_org_id, cost_org_id, quantity_value "
		"FROM adm_charge_item "
		"WHERE is_deleted = 0 AND service_id = ?";
	auto result = sqlSession->executeQuery<ChargeItemDO>(sql, ChargeItemSimpleMapper(), params);
	if (result.empty()) {
		return nullptr;
	}
	return std::make_unique<ChargeItemDO>(std::move(result.front()));
}

// 更新费用记录状态
int InpatientBillingDAO::updateChargeItemStatus(const std::string& id, const std::string& status)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, status);
	SQLPARAMS_PUSH(params, "s", std::string, id);
	std::string sql =
		"UPDATE adm_charge_item SET status_enum = ?, updated_at = NOW() "
		"WHERE id = ? AND is_deleted = 0";
	return sqlSession->executeUpdate(sql, params);
}