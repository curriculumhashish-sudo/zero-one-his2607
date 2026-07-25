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
#include "OrderExecuteControlDAO.h"
#include "ClnProcedureMapper.h"
#include "OriginalOrderRequestMapper.h"

namespace
{
	std::string buildPlaceholders(size_t count)
	{
		std::string placeholders;
		for (size_t i = 0; i < count; ++i)
		{
			if (i > 0)
				placeholders += ",";
			placeholders += "?";
		}
		return placeholders;
	}

	std::string buildSlotSecondConditions(size_t count)
	{
		std::string conditions;
		for (size_t i = 0; i < count; ++i)
		{
			if (i > 0)
				conditions += " OR ";
			conditions += "(request_id = ? AND request_table = ? AND occurrence_time >= ? AND occurrence_time < DATE_ADD(?, INTERVAL 1 SECOND))";
		}
		return conditions;
	}

	std::string buildValuesPlaceholders(size_t rowCount, size_t columnCount)
	{
		std::string placeholders;
		for (size_t row = 0; row < rowCount; ++row)
		{
			if (row > 0)
				placeholders += ", ";
			placeholders += "(";
			for (size_t column = 0; column < columnCount; ++column)
			{
				if (column > 0)
					placeholders += ", ";
				placeholders += "?";
			}
			placeholders += ")";
		}
		return placeholders;
	}

}

class EncounterIdMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString("encounter_id");
	}
};

class OriginalOrderRequestWithIdMapper : public Mapper<std::pair<std::string, PtrOriginalOrderRequestDO>>
{
public:
	std::pair<std::string, PtrOriginalOrderRequestDO> mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<OriginalOrderRequestDO>();
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setCategoryEnum(resultSet->getString("category_enum"));
		return std::make_pair(resultSet->getString("id"), data);
	}
};

std::list<std::pair<std::string, PtrOriginalOrderRequestDO>> OrderExecuteControlDAO::queryOriginalOrderRequestById(
	const std::list<std::string>& orderIds,
	const string& adviceType)
{
	if (orderIds.empty())
		return {};

	string table;
	if (adviceType == "MED_REQUEST")
		table = "med_medication_request";
	else if (adviceType == "SERVICE_REQUEST")
		table = "wkf_service_request";
	else
		return {};

	const string querySql = R"SQL(
		SELECT
			id,
			encounter_id,
			patient_id,
			category_enum
		FROM )SQL" +
		table +
		" WHERE id IN (" + buildPlaceholders(orderIds.size()) + ") AND is_deleted = 0;";

	SqlParams params;
	for (const auto& orderId : orderIds)
	{
		SQLPARAMS_PUSH(params, "s", string, orderId);
	}
	return sqlSession->executeQuery<std::pair<std::string, PtrOriginalOrderRequestDO>>(
		querySql,
		OriginalOrderRequestWithIdMapper(),
		params);
}

bool OrderExecuteControlDAO::existsActiveTerminalProcedureBySlot(
	const std::list<std::tuple<std::string, std::string, std::string>>& slots)
{
	if (slots.empty())
		return false;

	const string querySql = R"SQL(
		SELECT EXISTS (
			SELECT 1
			FROM cln_procedure
			WHERE (
			)SQL" +
				buildSlotSecondConditions(slots.size()) +
			R"SQL(
				)
				AND is_deleted = 0
				AND occurrence_slot_active = 1
				AND status_enum IN ('COMPLETED', 'STOPPED')
			LIMIT 1
		);
	)SQL";

	SqlParams params;
	for (const auto& slot : slots)
	{
		SQLPARAMS_PUSH(params, "s", string, std::get<0>(slot));
		SQLPARAMS_PUSH(params, "s", string, std::get<1>(slot));
		SQLPARAMS_PUSH(params, "dt", string, std::get<2>(slot));
		SQLPARAMS_PUSH(params, "dt", string, std::get<2>(slot));
	}
	return sqlSession->executeQueryNumerical(querySql, params) > 0;
}

std::list<PtrClnProcedureDO> OrderExecuteControlDAO::queryCompletedProcedureById(const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return {};

	const string querySql = R"SQL(
		SELECT
			id,
			tenant_id,
			version,
			request_id,
			request_table,
			status_enum,
			category_enum,
			encounter_id,
			patient_id,
			occurrence_time,
			recorder_id,
			org_id,
			location_id,
			group_id,
			refund_id,
			occurrence_slot_active
		FROM cln_procedure
		WHERE id IN (
		)SQL" +
			buildPlaceholders(procedureIds.size()) +
		R"SQL(
			)
			AND status_enum = 'COMPLETED'
			AND is_deleted = 0
			AND occurrence_slot_active = 1;
	)SQL";

	SqlParams params;
	for (const auto& procedureId : procedureIds)
	{
		SQLPARAMS_PUSH(params, "s", string, procedureId);
	}
	return sqlSession->executeQuery<PtrClnProcedureDO>(querySql, ClnProcedureCompletedMapper(), params);
}

bool OrderExecuteControlDAO::existsCancelProcedureByRefundIds(const std::list<std::string>& refundIds)
{
	if (refundIds.empty())
		return false;

	const string querySql = R"SQL(
		SELECT EXISTS (
			SELECT 1
			FROM cln_procedure
			WHERE refund_id IN (
		)SQL" +
			buildPlaceholders(refundIds.size()) +
		R"SQL(
			)
			AND status_enum = 'CANCEL'
			AND is_deleted = 0
			LIMIT 1
		);
	)SQL";

	SqlParams params;
	for (const auto& refundId : refundIds)
	{
		SQLPARAMS_PUSH(params, "s", string, refundId);
	}
	return sqlSession->executeQueryNumerical(querySql, params) > 0;
}

int OrderExecuteControlDAO::insertCancelProceduresNoTransaction(const std::list<ClnProcedureDO>& procedures)
{
	if (procedures.empty())
		return 0;

	const auto& firstProcedure = procedures.front();
	const size_t columnCount = firstProcedure.getFields().size() + 1;

	std::string columns = "(";
	columns += firstProcedure.getPrimaryField()->getColumn();
	for (const auto& field : firstProcedure.getFields())
	{
		columns += ", ";
		columns += field->getColumn();
	}
	columns += ")";

	SqlParams params;
	for (const auto& procedure : procedures)
	{
		if (!procedure.getPrimaryField()->get())
			throw std::runtime_error("Primary field is null.");

		extractFieldValueWithNul(procedure.getPrimaryField(), &params);
		for (const auto& field : procedure.getFields())
		{
			extractFieldValueWithNul(field, &params);
		}
	}

	const string insertSql = "INSERT INTO cln_procedure " +
		columns +
		" VALUES " +
		buildValuesPlaceholders(procedures.size(), columnCount) +
		";";

	return sqlSession->executeUpdate(insertSql, params);
}

int OrderExecuteControlDAO::releaseCompletedProcedureSlotsNoTransaction(const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return 0;

	const string updateSql = R"SQL(
		UPDATE cln_procedure
		SET occurrence_slot_active = 0
		WHERE id IN (
		)SQL" +
			buildPlaceholders(procedureIds.size()) +
		R"SQL(
			)
		)SQL";

	SqlParams params;
	for (const auto& procedureId : procedureIds)
	{
		SQLPARAMS_PUSH(params, "s", string, procedureId);
	}
	return sqlSession->executeUpdate(updateSql, params);
}

std::list<std::string> OrderExecuteControlDAO::listOperableEncounterIdsByUserId(const std::string& userId)
{
	const string querySql = R"SQL(
		SELECT DISTINCT
			el.encounter_id
		FROM adm_practitioner p
		JOIN adm_pr_assign a
			ON a.practitioner_id = p.id
			AND a.is_deleted = 0
			AND a.active_flag = 1
		JOIN adm_encounter_location el
			ON el.location_id = a.location_id
			AND el.is_deleted = 0
			AND el.status_enum = 'ACTIVE'
		JOIN adm_encounter e
			ON e.id = el.encounter_id
			AND e.is_deleted = 0
		WHERE p.user_id = ?
			AND p.is_deleted = 0
			AND p.active_flag = 1;
	)SQL";

	return sqlSession->executeQuery<std::string>(querySql, EncounterIdMapper(), "%s", userId);
}
