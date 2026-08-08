#include "stdafx.h"
#include "GetCostDetailDAO.h"
#include "GetCostDetailMapper.h"

namespace {
	// 日期下限归一化，yyyy-MM-dd 补全为当日零点
	std::string toLowerBound(const std::string &value) {
		return value.size() == 10 ? value + " 00:00:00" : value;
	}

	// 日期上限归一化，yyyy-MM-dd 补全为当日最后时刻
	std::string toUpperBound(const std::string &value) {
		return value.size() == 10 ? value + " 23:59:59" : value;
	}
}

std::string GetCostDetailDAO::selectItemSqlPrefix() {
	return
		"SELECT ci.id AS charge_item_id,"
		" ci.bus_no AS bus_no,"
		" ci.prescription_no AS prescription_no,"
		" COALESCE(cid.charge_name,ci.bus_no) AS item_name,"
		" cid.type_code AS charge_category_code,"
		" COALESCE(tc.display,cid.type_code) AS charge_category_name,"
		" ci.quantity_value AS quantity_value,"
		" ci.quantity_unit AS quantity_unit,"
		" ci.unit_price AS unit_price,"
		" ci.total_price AS total_price,"
		" ci.status_enum AS status_enum,"
		" ci.occurrence_time AS occurrence_time,"
		" org.name AS performing_org_name,"
		" enterer.name AS enterer_name"
		" FROM adm_charge_item ci"
		" LEFT JOIN adm_charge_item_definition cid"
		" ON cid.id=ci.definition_id AND cid.is_deleted=0"
		" LEFT JOIN adm_organization org"
		" ON org.id=ci.performing_org_id AND org.is_deleted=0"
		" LEFT JOIN adm_practitioner enterer"
		" ON enterer.id=ci.enterer_id AND enterer.is_deleted=0"
		" LEFT JOIN ter_code_system tcs"
		" ON tcs.name='fin_type_code'"
		" AND tcs.tenant_id=ci.tenant_id"
		" AND tcs.is_deleted=0"
		" LEFT JOIN ter_concept tc"
		" ON tc.code_system_id=tcs.id"
		" AND tc.code=cid.type_code"
		" AND tc.tenant_id=ci.tenant_id"
		" AND tc.is_active=1"
		" AND tc.is_deleted=0";
}

std::string GetCostDetailDAO::queryConditionBuilder(const GetCostDetailQuery::Wrapper& query, SqlParams& params) {
	std::stringstream sqlCondition;
	sqlCondition << " WHERE ci.is_deleted=0"
		<< " AND ci.encounter_id=?"
		<< " AND ci.patient_id=?";
	SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));

	const std::string statusEnum = query->statusEnum.getValue("");
	if (!statusEnum.empty() && statusEnum != "ALL") {
		sqlCondition << " AND ci.status_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, statusEnum);
	}

	// 费用发生日期区间过滤
	const std::string chargeDateFrom = query->chargeDateFrom.getValue("");
	if (!chargeDateFrom.empty()) {
		sqlCondition << " AND ci.occurrence_time>=?";
		SQLPARAMS_PUSH(params, "s", std::string, toLowerBound(chargeDateFrom));
	}
	const std::string chargeDateTo = query->chargeDateTo.getValue("");
	if (!chargeDateTo.empty()) {
		sqlCondition << " AND ci.occurrence_time<=?";
		SQLPARAMS_PUSH(params, "s", std::string, toUpperBound(chargeDateTo));
	}
	return sqlCondition.str();
}

PtrGetCostDetailDO GetCostDetailDAO::selectSummary(const GetCostDetailQuery::Wrapper& query) {
	SqlParams params;

	// 费用日期区间条件，与明细查询保持同一口径
	std::string dateCondition;
	const std::string chargeDateFrom = query->chargeDateFrom.getValue("");
	if (!chargeDateFrom.empty()) {
		dateCondition += " AND ci.occurrence_time>=?";
		SQLPARAMS_PUSH(params, "s", std::string, toLowerBound(chargeDateFrom));
	}
	const std::string chargeDateTo = query->chargeDateTo.getValue("");
	if (!chargeDateTo.empty()) {
		dateCondition += " AND ci.occurrence_time<=?";
		SQLPARAMS_PUSH(params, "s", std::string, toUpperBound(chargeDateTo));
	}

	const std::string sql =
		"SELECT p.id AS patient_id,"
		" p.name AS patient_name,"
		" e.id AS encounter_id,"
		" e.bus_no AS encounter_bus_no,"
		" COALESCE(SUM(CASE WHEN ci.status_enum IN ('DRAFT','UNSETTLED','SETTLED')"
		" THEN ci.total_price ELSE 0 END),0) AS total_amount,"
		" COALESCE(SUM(CASE WHEN ci.status_enum='SETTLED'"
		" THEN ci.total_price ELSE 0 END),0) AS settled_amount,"
		" COALESCE(SUM(CASE WHEN ci.status_enum IN ('DRAFT','UNSETTLED')"
		" THEN ci.total_price ELSE 0 END),0) AS unsettled_amount"
		" FROM adm_encounter e"
		" INNER JOIN adm_patient p"
		" ON p.id=e.patient_id AND p.is_deleted=0"
		" LEFT JOIN adm_charge_item ci"
		" ON ci.encounter_id=e.id"
		" AND ci.patient_id=p.id"
		" AND ci.is_deleted=0"
		+ dateCondition +
		" WHERE e.id=?"
		" AND e.patient_id=?"
		" AND e.is_deleted=0"
		" GROUP BY p.id,p.name,e.id,e.bus_no";

	SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	return sqlSession->executeQueryOne<PtrGetCostDetailDO>(sql, GetCostDetailMapper(), params);
}

std::list<PtrCostDetailItemDO> GetCostDetailDAO::selectItems(const GetCostDetailQuery::Wrapper& query) {
	std::string sql = GetCostDetailDAO::selectItemSqlPrefix();

	SqlParams params;
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY ci.occurrence_time ASC,ci.id ASC";
	return sqlSession->executeQuery<PtrCostDetailItemDO>(sql, CostDetailItemMapper(), params);
}
