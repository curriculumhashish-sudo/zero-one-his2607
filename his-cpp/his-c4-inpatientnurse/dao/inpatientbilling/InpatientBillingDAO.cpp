#include "stdafx.h"
#include "InpatientBillingDAO.h"
#include "Mapper.h"

namespace
{
class BillingStringMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};

void appendCatalogCondition(
	const PagePricingCatalogQuery::Wrapper& query, std::string& sql, SqlParams& params)
{
	sql += " WHERE 1=1";
	if (query->keyword && !query->keyword->empty()) {
		sql += " AND (x.name LIKE ? OR x.py_str LIKE ? OR x.wb_str LIKE ?)";
		const std::string keyword = "%" + query->keyword.getValue("") + "%";
		SQLPARAMS_PUSH(params, "s", std::string, keyword);
		SQLPARAMS_PUSH(params, "s", std::string, keyword);
		SQLPARAMS_PUSH(params, "s", std::string, keyword);
	}
	if (query->categoryCode && !query->categoryCode->empty()) {
		sql += " AND x.category_code=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->categoryCode.getValue(""));
	}
	if (query->organizationUnitId && !query->organizationUnitId->empty()) {
		sql += " AND x.org_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->organizationUnitId.getValue(""));
	}
}

std::string bindMarkers(size_t count)
{
	std::string value;
	for (size_t i = 0; i < count; ++i) {
		if (i > 0) value += ',';
		value += '?';
	}
	return value;
}

std::vector<std::string> splitEncounterIds(const oatpp::String& value)
{
	std::vector<std::string> ids;
	std::stringstream stream(value.getValue(""));
	std::string id;
	while (std::getline(stream, id, ',')) {
		const auto first = id.find_first_not_of(" \t\r\n");
		if (first == std::string::npos) continue;
		const auto last = id.find_last_not_of(" \t\r\n");
		ids.emplace_back(id.substr(first, last - first + 1));
	}
	return ids;
}

std::vector<std::string> resolveEncounterIds(
	const oatpp::String& encounterId, const oatpp::String& legacyEncounterIds)
{
	if (encounterId && !encounterId->empty()) return {encounterId.getValue("")};
	return splitEncounterIds(legacyEncounterIds);
}

class PricingCatalogMapper : public Mapper<NurseStationPricingCatalogRowDTO::Wrapper>
{
public:
	NurseStationPricingCatalogRowDTO::Wrapper mapper(ResultSet* rs) const override
	{
		auto row = NurseStationPricingCatalogRowDTO::createShared();
		row->productDefinitionId = rs->getString(1).asStdString();
		row->productTableName = rs->getString(2).asStdString();
		row->chargeItemDefinitionId = rs->getString(3).asStdString();
		row->name = rs->getString(4).asStdString();
		row->unitPrice = rs->getDouble(5);
		row->categoryCode = rs->getString(6).asStdString();
		row->categoryCodeText = rs->getString(6).asStdString();
		row->unitCode = rs->getString(7).asStdString();
		row->unitCodeText = rs->getString(7).asStdString();
		row->specificationText = rs->getString(8).asStdString();
		row->organizationUnitId = rs->getString(9).asStdString();
		row->organizationUnitName = rs->getString(10).asStdString();
		row->defaultDispenseLocationId = rs->getString(11).asStdString();
		return row;
	}
};

struct PricingReferenceRow
{
	double unitPrice = 0.0;
	std::string productTableName;
	std::string chargeItemDefinitionId;
};

class PricingReferenceMapper : public Mapper<PricingReferenceRow>
{
public:
	PricingReferenceRow mapper(ResultSet* rs) const override
	{
		PricingReferenceRow row;
		row.unitPrice = rs->getDouble(1);
		row.productTableName = rs->getString(2);
		row.chargeItemDefinitionId = rs->getString(3);
		return row;
	}
};

struct PricedOrderRow
{
	std::string encounterId;
	PricedOrderItemDTO::Wrapper item;
};

class PricedOrderMapper : public Mapper<PricedOrderRow>
{
public:
	PricedOrderRow mapper(ResultSet* rs) const override
	{
		PricedOrderRow row;
		row.encounterId = rs->getString(1);
		row.item = PricedOrderItemDTO::createShared();
		row.item->serviceRequestId = rs->getString(2).asStdString();
		row.item->requestId = rs->getString(2).asStdString();
		row.item->orderType = rs->getString(3).asStdString();
		row.item->productDefinitionId = rs->getString(4).asStdString();
		row.item->productName = rs->getString(5).asStdString();
		row.item->quantity = rs->getDouble(6);
		row.item->unitCode = rs->getString(7).asStdString();
		row.item->unitPrice = rs->getDouble(8);
		row.item->totalPrice = rs->getDouble(9);
		row.item->pricedTime = rs->getString(10).asStdString();
		row.item->pricingMode = rs->getString(11).asStdString();
		row.item->parentRequestId = rs->getString(12).asStdString();
		row.item->chargeStatusEnum = rs->getString(13).asStdString();
		row.item->dispenseStatusEnum = rs->getString(14).asStdString();
		return row;
	}
};

struct SupplementOrderRow
{
	std::string encounterId;
	SupplementParentOrderItemDTO::Wrapper item;
};

class SupplementOrderMapper : public Mapper<SupplementOrderRow>
{
public:
	SupplementOrderRow mapper(ResultSet* rs) const override
	{
		SupplementOrderRow row;
		row.encounterId = rs->getString(1);
		row.item = SupplementParentOrderItemDTO::createShared();
		row.item->orderId = rs->getString(2).asStdString();
		row.item->requestId = rs->getString(2).asStdString();
		row.item->orderName = rs->getString(3).asStdString();
		row.item->therapyType = rs->getString(4).asStdString();
		row.item->therapyTypeText = rs->getString(4).asStdString();
		row.item->dosageInstruction = rs->getString(5).asStdString();
		row.item->frequencyCode = rs->getString(6).asStdString();
		row.item->frequencyCodeText = rs->getString(6).asStdString();
		row.item->orderType = rs->getString(7).asStdString();
		row.item->statusEnum = rs->getString(8).asStdString();
		row.item->specificationText = rs->getString(9).asStdString();
		row.item->authoredTime = rs->getString(10).asStdString();
		row.item->stopTime = rs->getString(11).asStdString();
		row.item->requesterName = rs->getString(12).asStdString();
		row.item->performingLocationName = rs->getString(13).asStdString();
		return row;
	}
};
}

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

bool InpatientBillingDAO::isValidEncounterPatient(
	const std::string& encounterId, const std::string& patientId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM adm_encounter WHERE id=? AND patient_id=? "
		"AND is_deleted=0 AND status_enum IN ('IN_PROGRESS','ON_HOLD')",
		params) == 1;
}

bool InpatientBillingDAO::isValidAutoRuleReferences(
	const std::string& activityId, const std::string& chargeDefinitionId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, chargeDefinitionId);
	SQLPARAMS_PUSH(params, "s", std::string, activityId);
	SQLPARAMS_PUSH(params, "s", std::string, activityId);
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM wkf_activity_definition a "
		"JOIN adm_charge_item_definition d ON d.id=? "
		"AND d.instance_table='wkf_activity_definition' AND d.instance_id=? "
		"AND d.is_deleted=0 AND d.status_enum='ACTIVE' "
		"WHERE a.id=? AND a.is_deleted=0 AND a.status_enum='ACTIVE'",
		params) == 1;
}

bool InpatientBillingDAO::isActiveActivity(const std::string& activityId)
{
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM wkf_activity_definition "
		"WHERE id=? AND is_deleted=0 AND status_enum='ACTIVE'",
		"%s", activityId) == 1;
}

bool InpatientBillingDAO::isActiveChargeDefinition(const std::string& chargeDefinitionId)
{
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM adm_charge_item_definition "
		"WHERE id=? AND is_deleted=0 AND status_enum='ACTIVE'",
		"%s", chargeDefinitionId) == 1;
}

int InpatientBillingDAO::updateAutoGenRule(const UpdateAutoGenRuleDTO::Wrapper& dto)
{
	if (!dto || !dto->ruleId || dto->ruleId->empty()
		|| !dto->encounterId || dto->encounterId->empty()) return 0;
	SqlParams params;
	std::string sql = "UPDATE cus_charge_item_auto_gen SET updated_at=NOW(),version=version+1";
	if (dto->ruleName && !dto->ruleName->empty()) {
		sql += ",rule_name=?";
		SQLPARAMS_PUSH(params, "s", std::string, dto->ruleName.getValue(""));
	}
	if (dto->activityId && !dto->activityId->empty()) {
		sql += ",trigger_source_table='wkf_activity_definition',trigger_source_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, dto->activityId.getValue(""));
	}
	if (dto->chargeDefinitionId && !dto->chargeDefinitionId->empty()) {
		sql += ",charge_definition_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, dto->chargeDefinitionId.getValue(""));
	}
	if (dto->quantityValue) {
		sql += ",quantity_value=?";
		SQLPARAMS_PUSH(params, "d", double, dto->quantityValue.getValue(0.0));
	}
	if (dto->performingOrgId) {
		sql += ",performing_org_id=NULLIF(?,'')";
		SQLPARAMS_PUSH(params, "s", std::string, dto->performingOrgId.getValue(""));
	}
	if (dto->priority && !dto->priority->empty()) {
		int priority = 0;
		try { priority = std::stoi(dto->priority.getValue("0")); } catch (...) {}
		sql += ",priority=?";
		SQLPARAMS_PUSH(params, "i", int, priority);
	}
	if (dto->remark) {
		sql += ",remark=?";
		SQLPARAMS_PUSH(params, "s", std::string, dto->remark.getValue(""));
	}
	sql += " WHERE id=? AND encounter_id=? AND version=? AND is_deleted=0";
	SQLPARAMS_PUSH(params, "s", std::string, dto->ruleId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, dto->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "ll", long long, dto->expectedVersion.getValue(-1));
	return sqlSession->executeUpdate(sql, params);
}

int InpatientBillingDAO::softDeleteAutoGenRule(
	const std::string& ruleId, const std::string& encounterId,
	int64_t expectedVersion)
{
	return sqlSession->executeUpdate(
		"UPDATE cus_charge_item_auto_gen SET is_deleted=1,deleted_at=NOW(),"
		"updated_at=NOW(),version=version+1 WHERE id=? AND encounter_id=? "
		"AND version=? AND is_deleted=0",
		"%s%s%lld", ruleId, encounterId, static_cast<long long>(expectedVersion));
}

int InpatientBillingDAO::setAutoGenRuleStatus(
	const std::string& ruleId, const std::string& encounterId, const std::string& status,
	int64_t expectedVersion)
{
	return sqlSession->executeUpdate(
		"UPDATE cus_charge_item_auto_gen SET status_enum=?,updated_at=NOW(),version=version+1 "
		"WHERE id=? AND encounter_id=? AND version=? AND is_deleted=0",
		"%s%s%s%lld", status, ruleId, encounterId,
		static_cast<long long>(expectedVersion));
}

uint64_t InpatientBillingDAO::countPricingCatalog(const PagePricingCatalogQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT COUNT(*) FROM ("
		"SELECT a.name,a.py_str,a.wb_str,a.category_code,COALESCE(d.org_id,a.org_id,'') org_id "
		"FROM adm_charge_item_definition d JOIN wkf_activity_definition a "
		"ON a.id=d.instance_id AND d.instance_table='wkf_activity_definition' "
		"WHERE d.is_deleted=0 AND d.status_enum='ACTIVE' "
		"AND a.is_deleted=0 AND a.status_enum='ACTIVE' UNION ALL "
		"SELECT v.name,v.py_str,v.wb_str,v.category_code,COALESCE(d.org_id,v.org_id,'') "
		"FROM adm_charge_item_definition d JOIN adm_device_definition v "
		"ON v.id=d.instance_id AND d.instance_table='adm_device_definition' "
		"WHERE d.is_deleted=0 AND d.status_enum='ACTIVE' "
		"AND v.is_deleted=0 AND v.status_enum='ACTIVE' AND v.active_flag=1) x";
	appendCatalogCondition(query, sql, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<NurseStationPricingCatalogRowDTO::Wrapper> InpatientBillingDAO::selectPricingCatalog(
	const PagePricingCatalogQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT x.product_id,x.product_table,x.definition_id,x.name,x.price_value,"
		"x.category_code,x.unit_code,x.specification_text,x.org_id,x.org_name,"
		"x.default_location_id FROM ("
		"SELECT a.id product_id,'wkf_activity_definition' product_table,d.id definition_id,"
		"a.name,d.price_value,COALESCE(a.category_code,'') category_code,"
		"COALESCE(a.def_inhospital_unit_code,a.package_unit_code,'') unit_code,"
		"COALESCE(a.description_text,'') specification_text,"
		"COALESCE(d.org_id,a.org_id,'') org_id,COALESCE(o.name,'') org_name,"
		"'' default_location_id,d.priority sort_priority,a.py_str,a.wb_str "
		"FROM adm_charge_item_definition d JOIN wkf_activity_definition a "
		"ON a.id=d.instance_id AND d.instance_table='wkf_activity_definition' "
		"LEFT JOIN adm_organization o ON o.id=COALESCE(d.org_id,a.org_id) AND o.is_deleted=0 "
		"WHERE d.is_deleted=0 AND d.status_enum='ACTIVE' "
		"AND a.is_deleted=0 AND a.status_enum='ACTIVE' UNION ALL "
		"SELECT v.id,'adm_device_definition',d.id,v.name,d.price_value,"
		"COALESCE(v.category_code,''),"
		"COALESCE(v.def_inhospital_unit_code,v.stock_unit_code,v.package_unit_code,''),"
		"COALESCE(v.model_number,v.description,''),"
		"COALESCE(d.org_id,v.org_id,''),COALESCE(o.name,''),COALESCE(v.location_id,''),"
		"d.priority,v.py_str,v.wb_str "
		"FROM adm_charge_item_definition d JOIN adm_device_definition v "
		"ON v.id=d.instance_id AND d.instance_table='adm_device_definition' "
		"LEFT JOIN adm_organization o ON o.id=COALESCE(d.org_id,v.org_id) AND o.is_deleted=0 "
		"WHERE d.is_deleted=0 AND d.status_enum='ACTIVE' "
		"AND v.is_deleted=0 AND v.status_enum='ACTIVE' AND v.active_flag=1) x";
	appendCatalogCondition(query, sql, params);
	sql += " ORDER BY x.sort_priority,x.definition_id LIMIT ? OFFSET ?";
	SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));
	SQLPARAMS_PUSH(params, "ull", uint64_t,
		(query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
	return sqlSession->executeQuery<NurseStationPricingCatalogRowDTO::Wrapper>(
		sql, PricingCatalogMapper(), params);
}

bool InpatientBillingDAO::prepareNursePricingItem(
	const ConfirmDirectPricingItemDTO::Wrapper& item)
{
	if (!item || !item->productDefinitionId || item->productDefinitionId->empty()
		|| !item->unitCode || item->unitCode->empty())
		return false;
	std::string sql =
		"SELECT d.price_value,d.instance_table,d.id FROM adm_account ac "
		"JOIN adm_encounter e ON e.id=? AND e.patient_id=? AND e.is_deleted=0 "
		"AND e.status_enum IN ('IN_PROGRESS','ON_HOLD') "
		"JOIN adm_charge_item_definition d ON d.instance_id=? "
		"AND d.is_deleted=0 AND d.status_enum='ACTIVE' "
		"WHERE ac.id=? AND ac.encounter_id=? AND ac.patient_id=? "
		"AND ac.is_deleted=0 AND ac.status_enum='ACTIVE' AND ("
		"(d.instance_table='wkf_activity_definition' AND EXISTS(SELECT 1 "
		"FROM wkf_activity_definition a WHERE a.id=d.instance_id AND a.is_deleted=0 "
		"AND a.status_enum='ACTIVE' AND COALESCE(a.def_inhospital_unit_code,a.package_unit_code,'')=?)) "
		"OR (d.instance_table='adm_device_definition' AND EXISTS(SELECT 1 "
		"FROM adm_device_definition v WHERE v.id=d.instance_id AND v.is_deleted=0 "
		"AND v.status_enum='ACTIVE' AND v.active_flag=1 "
		"AND COALESCE(v.def_inhospital_unit_code,v.stock_unit_code,v.package_unit_code,'')=?)))";
	SqlParams ordered;
	SQLPARAMS_PUSH(ordered, "s", std::string, item->encounterId.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->patientId.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->productDefinitionId.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->accountId.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->encounterId.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->patientId.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->unitCode.getValue(""));
	SQLPARAMS_PUSH(ordered, "s", std::string, item->unitCode.getValue(""));
	auto rows = sqlSession->executeQuery<PricingReferenceRow>(
		sql, PricingReferenceMapper(), ordered);
	if (rows.size() != 1) return false;
	if (rows.front().productTableName == "adm_device_definition") {
		if (!item->performLocation || item->performLocation->empty()) return false;
		if (sqlSession->executeQueryNumerical(
			"SELECT COUNT(*) FROM adm_location WHERE id=? AND is_deleted=0 AND status_enum='ENABLE'",
			"%s", item->performLocation.getValue("")) != 1) return false;
	}
	item->productTableName = rows.front().productTableName;
	item->chargeItemDefinitionId = rows.front().chargeItemDefinitionId;
	item->unitPrice = rows.front().unitPrice;
	item->totalPrice = rows.front().unitPrice * item->quantity.getValue(0.0);
	return true;
}

bool InpatientBillingDAO::isAllowedSupplementParent(
	const std::string& requestId, const std::string& encounterId,
	const std::string& orderType)
{
	if (orderType != "SERVICE_REQUEST" && orderType != "MED_REQUEST") return false;
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	const std::string table = orderType == "MED_REQUEST"
		? "med_medication_request" : "wkf_service_request";
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM " + table + " WHERE id=? AND encounter_id=? "
		"AND is_deleted=0 AND generate_source_enum='DOCTOR_PRESCRIPTION' "
		"AND status_enum IN ('VERIFIED','STOPPED')",
		params) == 1;
}

std::list<PricedOrderGroupDTO::Wrapper> InpatientBillingDAO::selectPricedOrders(
	const ListPricedOrdersQuery::Wrapper& query)
{
	const auto encounterIds = resolveEncounterIds(query->encounterId, query->encounterIds);
	if (encounterIds.empty()) return {};
	SqlParams params;
	std::string sql =
		"SELECT ci.encounter_id,ci.service_id,"
		"CASE WHEN ci.service_table='wkf_device_request' THEN 'DEVICE_REQUEST' "
		"ELSE 'SERVICE_REQUEST' END,ci.product_id,"
		"COALESCE(d.charge_name,''),ci.quantity_value,ci.quantity_unit,"
		"ci.unit_price,ci.total_price,ci.created_at,"
		"CASE WHEN COALESCE(sr.parent_id,dr.based_on_id,'')='' THEN 'DIRECT' "
		"ELSE 'SUPPLEMENT' END,COALESCE(sr.parent_id,dr.based_on_id,''),"
		"COALESCE(ci.status_enum,''),COALESCE(dd.status_enum,'') "
		"FROM adm_charge_item ci "
		"LEFT JOIN adm_charge_item_definition d ON d.id=ci.definition_id AND d.is_deleted=0 "
		"LEFT JOIN wkf_service_request sr ON sr.id=ci.service_id "
		"AND ci.service_table='wkf_service_request' "
		"LEFT JOIN wkf_device_request dr ON dr.id=ci.service_id "
		"AND ci.service_table='wkf_device_request' "
		"LEFT JOIN wkf_device_dispense dd ON dd.id=ci.dispense_id AND dd.is_deleted=0 "
		"WHERE ci.is_deleted=0 AND ci.generate_source_enum='NURSE_PRICING' "
		"AND ci.encounter_id IN (" + bindMarkers(encounterIds.size()) + ")";
	for (const auto& encounterId : encounterIds) {
		SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	}
	if (query->pricedTimeFrom && !query->pricedTimeFrom->empty()) {
		sql += " AND ci.created_at>=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->pricedTimeFrom.getValue(""));
	}
	if (query->pricedTimeTo && !query->pricedTimeTo->empty()) {
		sql += " AND ci.created_at<=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->pricedTimeTo.getValue(""));
	}
	sql += " ORDER BY ci.encounter_id,ci.created_at DESC,ci.id DESC";
	auto rows = sqlSession->executeQuery<PricedOrderRow>(sql, PricedOrderMapper(), params);
	std::list<PricedOrderGroupDTO::Wrapper> groups;
	std::map<std::string, PricedOrderGroupDTO::Wrapper> index;
	for (auto& row : rows) {
		auto& group = index[row.encounterId];
		if (!group) {
			group = PricedOrderGroupDTO::createShared();
			group->encounterId = row.encounterId;
			group->pricedOrders = oatpp::List<PricedOrderItemDTO::Wrapper>::createShared();
			groups.push_back(group);
		}
		group->pricedOrders->push_back(row.item);
	}
	return groups;
}

std::list<SupplementParentOrderGroupDTO::Wrapper> InpatientBillingDAO::selectSupplementParentOrders(
	const ListSupplementParentOrdersQuery::Wrapper& query)
{
	const auto encounterIds = resolveEncounterIds(query->encounterId, query->encounterIds);
	if (encounterIds.empty()) return {};
	SqlParams params;
	std::string sql =
		"SELECT x.encounter_id,x.order_id,x.order_name,x.therapy_type,"
		"x.dosage_instruction,x.frequency_code,x.order_type,x.status_enum,"
		"x.specification_text,x.authored_time,x.stop_time,x.requester_name,"
		"x.location_name FROM ("
		"SELECT sr.encounter_id,sr.id order_id,COALESCE(a.name,sr.remark,'') order_name,"
		"COALESCE(sr.therapy_enum,'') therapy_type,'' dosage_instruction,"
		"COALESCE(sr.rate_code,'') frequency_code,COALESCE(sr.org_id,'') org_id,"
		"'SERVICE_REQUEST' order_type,sr.status_enum,COALESCE(a.description_text,''),"
		"sr.authored_time,COALESCE(sr.occurrence_end_time,''),COALESCE(p.name,''),"
		"COALESCE(l.name,'') "
		"FROM wkf_service_request sr "
		"LEFT JOIN wkf_activity_definition a ON a.id=sr.activity_id AND a.is_deleted=0 "
		"LEFT JOIN adm_practitioner p ON p.id=sr.requester_id AND p.is_deleted=0 "
		"LEFT JOIN adm_location l ON l.id=sr.location_id AND l.is_deleted=0 "
		"WHERE sr.is_deleted=0 AND sr.generate_source_enum='DOCTOR_PRESCRIPTION' "
		"UNION ALL "
		"SELECT mr.encounter_id,mr.id,COALESCE(m.name,mr.remark,''),"
		"COALESCE(mr.therapy_enum,''),COALESCE(mr.dosage_instruction,''),"
		"COALESCE(mr.rate_code,''),COALESCE(mr.org_id,''),'MED_REQUEST',"
		"mr.status_enum,COALESCE(m.total_volume,''),mr.req_authored_time,"
		"COALESCE(mr.effective_dose_end,''),COALESCE(p.name,''),COALESCE(l.name,'') "
		"FROM med_medication_request mr "
		"LEFT JOIN med_medication_definition m ON m.id=mr.medication_id AND m.is_deleted=0 "
		"LEFT JOIN adm_practitioner p ON p.id=mr.requester_id AND p.is_deleted=0 "
		"LEFT JOIN adm_location l ON l.id=mr.location_id AND l.is_deleted=0 "
		"WHERE mr.is_deleted=0 AND mr.generate_source_enum='DOCTOR_PRESCRIPTION') x "
		"WHERE x.encounter_id IN (" + bindMarkers(encounterIds.size()) + ")";
	for (const auto& encounterId : encounterIds) {
		SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	}
	if (query->therapyType && !query->therapyType->empty()) {
		sql += " AND x.therapy_type=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->therapyType.getValue(""));
	}
	if (query->keyword && !query->keyword->empty()) {
		sql += " AND x.order_name LIKE ?";
		SQLPARAMS_PUSH(params, "s", std::string, "%" + query->keyword.getValue("") + "%");
	}
	if (query->orgId && !query->orgId->empty()) {
		sql += " AND x.org_id=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->orgId.getValue(""));
	}
	sql += " ORDER BY x.encounter_id,x.order_id DESC";
	auto rows = sqlSession->executeQuery<SupplementOrderRow>(sql, SupplementOrderMapper(), params);
	std::list<SupplementParentOrderGroupDTO::Wrapper> groups;
	std::map<std::string, SupplementParentOrderGroupDTO::Wrapper> index;
	for (auto& row : rows) {
		auto& group = index[row.encounterId];
		if (!group) {
			group = SupplementParentOrderGroupDTO::createShared();
			group->encounterId = row.encounterId;
			group->orders = oatpp::List<SupplementParentOrderItemDTO::Wrapper>::createShared();
			groups.push_back(group);
		}
		group->orders->push_back(row.item);
	}
	return groups;
}

int InpatientBillingDAO::insertNursePricingServiceRequest(
	const std::string& requestId,
	const ConfirmDirectPricingItemDTO::Wrapper& item,
	const std::string& parentRequestId,
	const std::string& operatorId,
	const std::string& now)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, item->productDefinitionId.getValue(""));
	SQLPARAMS_PUSH(params, "d", double, item->quantity.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, item->unitCode.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->patientId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, parentRequestId);
	SQLPARAMS_PUSH(params, "s", std::string, item->remark.getValue(""));
	return sqlSession->executeUpdate(
		"INSERT INTO wkf_service_request(id,created_at,created_by,is_deleted,version,"
		"status_enum,category_enum,activity_id,quantity,unit_code,patient_id,encounter_id,"
		"authored_time,requester_id,parent_id,generate_source_enum,remark) "
		"VALUES(?,?,?,0,0,'VERIFIED','NURSE_PRICING',?,?,?,?,?,?,?,NULLIF(?,''),"
		"'NURSE_PRICING',?)", params);
}

int InpatientBillingDAO::insertNursePricingDeviceRequest(
	const std::string& requestId,
	const ConfirmDirectPricingItemDTO::Wrapper& item,
	const std::string& parentRequestId,
	const std::string& operatorId,
	const std::string& now)
{
	long long performLocation = 0;
	try { performLocation = std::stoll(item->performLocation.getValue("")); }
	catch (...) { return 0; }
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "d", double, item->quantity.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, item->unitCode.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->productDefinitionId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "ll", long long, performLocation);
	SQLPARAMS_PUSH(params, "s", std::string, item->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->patientId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, parentRequestId);
	SQLPARAMS_PUSH(params, "s", std::string, parentRequestId);
	SQLPARAMS_PUSH(params, "s", std::string, parentRequestId);
	SQLPARAMS_PUSH(params, "s", std::string, item->remark.getValue(""));
	return sqlSession->executeUpdate(
		"INSERT INTO wkf_device_request(id,created_at,created_by,is_deleted,version,bus_no,"
		"status_enum,category_enum,perform_flag,quantity,unit_code,device_def_id,requester_id,"
		"perform_location,encounter_id,patient_id,req_authored_time,performer_enum,performer_id,"
		"based_on_table,based_on_id,generate_source_enum,remark) VALUES(?,?,?,0,0,?,"
		"'COMPLETED','NURSE_PRICING',1,?,?,?,?,?,?,?,?, 'PRACTITIONER',?,"
		"CASE WHEN ?='' THEN NULL WHEN EXISTS(SELECT 1 FROM med_medication_request "
		"WHERE id=? AND is_deleted=0) THEN 'med_medication_request' "
		"ELSE 'wkf_service_request' END,NULLIF(?,''),'NURSE_PRICING',?)", params);
}

int InpatientBillingDAO::insertNursePricingDeviceDispense(
	const std::string& dispenseId,
	const std::string& requestId,
	const ConfirmDirectPricingItemDTO::Wrapper& item,
	const std::string& parentRequestId,
	const std::string& operatorId,
	const std::string& now)
{
	(void)parentRequestId;
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, dispenseId);
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, dispenseId);
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "d", double, item->quantity.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, item->unitCode.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->productDefinitionId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->patientId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, item->performLocation.getValue(""));
	SQLPARAMS_PUSH(params, "d", double, item->quantity.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, item->performLocation.getValue(""));
	return sqlSession->executeUpdate(
		"INSERT INTO wkf_device_dispense(id,created_at,created_by,is_deleted,version,bus_no,"
		"device_req_id,status_enum,based_on_table,based_on_id,dispense_category_enum,quantity,"
		"unit_code,device_def_id,patient_id,encounter_id,performer_id,location_id,"
		"dispense_quantity,preparer_id,prepare_time,dispense_time,des_location_id) "
		"VALUES(?,?,?,0,0,?,?,'COMPLETED','wkf_device_request',?,"
		"'NURSE_PRICING',?,?,?,?,?,?,?,?,?,?,?,?)", params);
}

int InpatientBillingDAO::insertNursePricingChargeItem(
	const std::string& chargeItemId,
	const std::string& requestId,
	const std::string& dispenseId,
	const ConfirmDirectPricingItemDTO::Wrapper& item,
	const std::string& operatorId,
	const std::string& now)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, chargeItemId);
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, item->patientId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->encounterId.getValue(""));
	const bool device = item->productTableName == "adm_device_definition";
	SQLPARAMS_PUSH(params, "s", std::string,
		device ? "wkf_device_request" : "wkf_service_request");
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, item->accountId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->chargeItemDefinitionId.getValue(""));
	SQLPARAMS_PUSH(params, "d", double, item->totalPrice.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "d", double, item->quantity.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, item->unitCode.getValue(""));
	SQLPARAMS_PUSH(params, "d", double, item->unitPrice.getValue(0.0));
	SQLPARAMS_PUSH(params, "s", std::string, item->productTableName.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, item->productDefinitionId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string,
		device ? "wkf_device_dispense" : "");
	SQLPARAMS_PUSH(params, "s", std::string, dispenseId);
	return sqlSession->executeUpdate(
		"INSERT INTO adm_charge_item(id,created_at,created_by,is_deleted,version,"
		"generate_source_enum,context_enum,patient_id,encounter_id,service_table,service_id,"
		"account_id,definition_id,status_enum,total_price,occurrence_time,quantity_value,"
		"quantity_unit,unit_price,product_table,product_id,dispense_table,dispense_id) "
		"VALUES(?,?,?,0,0,'NURSE_PRICING','INPATIENT',?,?,?,?,?,?,"
		"'PENDING_SETTLEMENT',?,?,?,?,?,?,?,NULLIF(?,''),NULLIF(?,''))", params);
}

int InpatientBillingDAO::softDeleteNursePricingRequest(
	const std::string& orderType,
	const std::string& requestId,
	const std::string& encounterId,
	const std::string& operatorId,
	const std::string& now)
{
	if (orderType != "DEVICE_REQUEST" && orderType != "SERVICE_REQUEST"
		&& orderType != "DIRECT") return 0;
	bool device = orderType == "DEVICE_REQUEST";
	if (orderType == "DIRECT")
	{
		const auto tables = sqlSession->executeQuery<std::string>(
			"SELECT service_table FROM adm_charge_item WHERE service_id=? "
			"AND encounter_id=? AND generate_source_enum='NURSE_PRICING' "
			"AND is_deleted=0 LIMIT 1", BillingStringMapper(),
			"%s%s", requestId, encounterId);
		if (tables.empty()) return 0;
		device = tables.front() == "wkf_device_request";
	}
	const std::string table = device ? "wkf_device_request" : "wkf_service_request";
	if (device) {
		SqlParams dispenseParams;
		SQLPARAMS_PUSH(dispenseParams, "s", std::string, now);
		SQLPARAMS_PUSH(dispenseParams, "s", std::string, operatorId);
		SQLPARAMS_PUSH(dispenseParams, "s", std::string, requestId);
		SQLPARAMS_PUSH(dispenseParams, "s", std::string, encounterId);
		sqlSession->executeUpdate(
			"UPDATE wkf_device_dispense SET is_deleted=1,deleted_at=?,deleted_by=?,"
			"version=version+1 WHERE device_req_id=? AND encounter_id=? AND is_deleted=0",
			dispenseParams);
	}
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	return sqlSession->executeUpdate(
		"UPDATE " + table + " SET is_deleted=1,deleted_at=?,deleted_by=?,version=version+1 "
		"WHERE id=? AND encounter_id=? AND is_deleted=0 "
		"AND generate_source_enum='NURSE_PRICING'", params);
}

int InpatientBillingDAO::softDeletePendingChargeItems(
	const std::string& requestId,
	const std::string& encounterId,
	const std::string& operatorId,
	const std::string& now)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, now);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	return sqlSession->executeUpdate(
		"UPDATE adm_charge_item SET is_deleted=1,deleted_at=?,deleted_by=?,version=version+1 "
		"WHERE service_id=? AND encounter_id=? AND is_deleted=0 "
		"AND generate_source_enum='NURSE_PRICING' AND status_enum='PENDING_SETTLEMENT'", params);
}

std::list<std::string> InpatientBillingDAO::selectIdempotencyResult(
	const std::string& operation, const std::string& idempotencyKey)
{
	return sqlSession->executeQuery<std::string>(R"SQL(
		SELECT changes FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_INPATIENT_BILLING'
			AND entity_id=? AND operation_type=?
		ORDER BY created_at DESC LIMIT 1
	)SQL", BillingStringMapper(), "%s%s", idempotencyKey, operation);
}

int InpatientBillingDAO::insertIdempotencyResult(
	const std::string& auditId, const std::string& operation,
	const std::string& idempotencyKey, const std::string& operatorId,
	const std::string& fingerprint, const std::string& resultData)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO whale_audit_logs(
			id,entity_type,entity_id,operation_type,user_id,changes,created_at)
		VALUES(?,'NURSE_STATION_INPATIENT_BILLING',?,?,?,?,NOW())
	)SQL", "%s%s%s%s%s", auditId, idempotencyKey, operation, operatorId,
		fingerprint + "\n--RESULT--\n" + resultData);
}

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

uint64_t InpatientBillingDAO::countChargeItemsByPatientId(const std::string& patientId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	std::string sql =
		"SELECT COUNT(*) FROM adm_charge_item "
		"WHERE is_deleted = 0 AND patient_id = ?";
	return sqlSession->executeQueryNumerical(sql, params);
}

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
