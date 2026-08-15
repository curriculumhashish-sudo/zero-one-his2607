/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "CostDetailDAO.h"
#include "Mapper.h"

class CostDetailMapper : public Mapper<CostDetailDTO::Wrapper>
{
public:
	CostDetailDTO::Wrapper mapper(ResultSet* resultSet) const override
	{
		auto data = CostDetailDTO::createShared();
		data->id = resultSet->getString(1).asStdString();
		data->patientId = resultSet->getString(2).asStdString();
		data->encounterId = resultSet->getString(3).asStdString();
		data->chargeName = resultSet->getString(4).asStdString();
		data->typeCode = resultSet->getString(5).asStdString();
		data->unitPrice = resultSet->getDouble(6);
		data->quantityValue = resultSet->getDouble(7);
		data->quantityUnit = resultSet->getString(8).asStdString();
		data->totalPrice = resultSet->getDouble(9);
		data->baseAmount = resultSet->getDouble(10);
		data->discountAmount = resultSet->getDouble(11);
		data->occurrenceTime = resultSet->getString(12).asStdString();
		data->performingOrgId = resultSet->getString(13).asStdString();
		data->statusEnum = resultSet->getString(14).asStdString();
		data->busNo = resultSet->getString(15).asStdString();
		return data;
	}
};

class CostSummaryMapper : public Mapper<CostSummaryDTO::Wrapper>
{
public:
	CostSummaryDTO::Wrapper mapper(ResultSet* resultSet) const override
	{
		auto data = CostSummaryDTO::createShared();
		data->typeCode = resultSet->getString(1).asStdString();
		data->typeName = resultSet->getString(2).asStdString();
		data->totalAmount = resultSet->getDouble(3);
		data->count = resultSet->getInt64(4);
		data->summaryDate = resultSet->getString(5).asStdString();
		return data;
	}
};

std::string CostDetailDAO::queryConditionBuilder(const CostDetailQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sql;
	sql << " WHERE i.is_deleted = 0";
	sql << " AND COALESCE(i.status_enum,'') NOT IN ('CANCEL','CANCELLED','VOID','REVOKED')";
	sql << " AND i.patient_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	sql << " AND i.encounter_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	if (query->startDate && !query->startDate->empty()) {
		sql << " AND i.occurrence_time >= ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
	}
	if (query->endDate && !query->endDate->empty()) {
		if (query->endDate->size() == 10) {
			sql << " AND i.occurrence_time < DATE_ADD(?, INTERVAL 1 DAY)";
		}
		else {
			sql << " AND i.occurrence_time <= ?";
		}
		SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
	}
	if (query->typeCode && !query->typeCode->empty()) {
		sql << " AND d.type_code = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->typeCode.getValue(""));
	}
	if (query->busNo && !query->busNo->empty()) {
		sql << " AND i.bus_no = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->busNo.getValue(""));
	}
	return sql.str();
}

uint64_t CostDetailDAO::count(const CostDetailQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_charge_item i LEFT JOIN adm_charge_item_definition d ON i.definition_id = d.id ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<CostDetailDTO::Wrapper> CostDetailDAO::selectWithPage(const CostDetailQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT i.id, i.patient_id, i.encounter_id, "
		"COALESCE(NULLIF(d.charge_name,''),medication.name,activity.name,''), d.type_code, "
		"i.unit_price, i.quantity_value, i.quantity_unit, i.total_price, "
		"i.base_amount, i.discount_amount, i.occurrence_time, "
		"i.performing_org_id, i.status_enum, i.bus_no "
		"FROM adm_charge_item i "
		"LEFT JOIN adm_charge_item_definition d ON i.definition_id = d.id "
		"LEFT JOIN med_medication_definition medication "
		"ON i.product_table='med_medication_definition' AND medication.id=i.product_id "
		"AND medication.is_deleted=0 "
		"LEFT JOIN wkf_activity_definition activity "
		"ON i.product_table='wkf_activity_definition' AND activity.id=i.product_id "
		"AND activity.is_deleted=0 ";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY i.occurrence_time DESC ";
	sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize) + "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<CostDetailDTO::Wrapper>(sql, CostDetailMapper(), params);
}

std::list<CostSummaryDTO::Wrapper> CostDetailDAO::selectSummary(const CostSummaryQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT d.type_code, COALESCE(tc.display,d.type_code), "
		"COALESCE(SUM(i.total_price), 0), COUNT(*), "
		"DATE(i.occurrence_time) "
		"FROM adm_charge_item i "
		"LEFT JOIN adm_charge_item_definition d ON i.definition_id = d.id "
		"LEFT JOIN ter_concept tc ON tc.code_system_id='236' AND tc.code=d.type_code AND tc.is_deleted=0 "
		"WHERE i.is_deleted = 0 "
		"AND COALESCE(i.status_enum,'') NOT IN ('CANCEL','CANCELLED','VOID','REVOKED') "
		"AND i.patient_id = ? "
		"AND i.encounter_id = ? ";
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	if (query->startDate && !query->startDate->empty()) {
		sql += "AND i.occurrence_time >= ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
	}
	if (query->endDate && !query->endDate->empty()) {
		if (query->endDate->size() == 10) {
			sql += "AND i.occurrence_time < DATE_ADD(?, INTERVAL 1 DAY) ";
		}
		else {
			sql += "AND i.occurrence_time <= ? ";
		}
		SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
	}
	sql += "GROUP BY d.type_code,tc.display,DATE(i.occurrence_time) ORDER BY d.type_code";
	return sqlSession->executeQuery<CostSummaryDTO::Wrapper>(sql, CostSummaryMapper(), params);
}
