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
		data->insuranceAmount = resultSet->getDouble(5);
		data->selfPayAmount = resultSet->getDouble(6);
		data->summaryDate = resultSet->getString(7).asStdString();
		return data;
	}
};

std::string CostDetailDAO::queryConditionBuilder(const CostDetailQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sql;
	sql << " WHERE i.is_deleted = 0";
	sql << " AND i.patient_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	sql << " AND i.encounter_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	sql << " AND i.occurrence_time >= ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
	sql << " AND i.occurrence_time <= ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
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
		"SELECT i.id, i.patient_id, i.encounter_id, d.charge_name, d.type_code, "
		"i.unit_price, i.quantity_value, i.quantity_unit, i.total_price, "
		"i.base_amount, i.discount_amount, i.occurrence_time, "
		"i.performing_org_id, i.status_enum, i.bus_no "
		"FROM adm_charge_item i "
		"LEFT JOIN adm_charge_item_definition d ON i.definition_id = d.id ";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY i.occurrence_time DESC ";
	sql += " LIMIT " + std::to_string((query->pageIndex - 1) * query->pageSize) + "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<CostDetailDTO::Wrapper>(sql, CostDetailMapper(), params);
}

std::list<CostSummaryDTO::Wrapper> CostDetailDAO::selectSummary(const CostSummaryQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT d.type_code, d.type_code, "
		"COALESCE(SUM(i.total_price), 0), COUNT(*), 0, COALESCE(SUM(i.total_price), 0), "
		"DATE(i.occurrence_time) "
		"FROM adm_charge_item i "
		"LEFT JOIN adm_charge_item_definition d ON i.definition_id = d.id "
		"WHERE i.is_deleted = 0 "
		"AND i.encounter_id = ? "
		"AND i.occurrence_time >= ? "
		"AND i.occurrence_time <= ? "
		"GROUP BY d.type_code, DATE(i.occurrence_time) "
		"ORDER BY d.type_code";
	SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
	return sqlSession->executeQuery<CostSummaryDTO::Wrapper>(sql, CostSummaryMapper(), params);
}
