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

	void pushStringParams(SqlParams& params, const std::list<std::string>& values)
	{
		for (const auto& value : values)
		{
			SQLPARAMS_PUSH(params, "s", string, value);
		}
	}

	std::string buildChargeRelationCondition(
		const std::list<std::string>& procedureIds,
		const std::list<std::string>& serviceRequestIds,
		SqlParams& params)
	{
		std::string condition = "(";
		bool hasCondition = false;
		if (!procedureIds.empty())
		{
			condition += "procedure_id IN (" + buildPlaceholders(procedureIds.size()) + ")";
			pushStringParams(params, procedureIds);
			hasCondition = true;
		}
		if (!serviceRequestIds.empty())
		{
			if (hasCondition)
				condition += " OR ";
			condition += "service_id IN (" + buildPlaceholders(serviceRequestIds.size()) + ")";
			pushStringParams(params, serviceRequestIds);
			hasCondition = true;
		}
		condition += ")";
		return hasCondition ? condition : "1=0";
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

class StringColumnMapper : public Mapper<std::string>
{
private:
	std::string columnName;
public:
	explicit StringColumnMapper(std::string column) : columnName(std::move(column)) {}

	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(columnName);
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

class MedicationDispenseForCancelMapper : public Mapper<PtrMedicationDispenseDO>
{
public:
	PtrMedicationDispenseDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<MedicationDispenseDO>();
		data->setId(resultSet->getString("id"));
		if (!resultSet->isNull("tenant_id")) data->setTenantId(resultSet->getString("tenant_id"));
		data->setBusNo(resultSet->getString("bus_no"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setDispenseEnum(resultSet->getString("dispense_enum"));
		data->setMedicationId(resultSet->getString("medication_id"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setLocationId(resultSet->getString("location_id"));
		data->setMedReqId(resultSet->getString("med_req_id"));
		data->setQuantity(resultSet->getDouble("quantity"));
		data->setUnitCode(resultSet->getString("unit_code"));
		if (!resultSet->isNull("dispense_quantity")) data->setDispenseQuantity(resultSet->getDouble("dispense_quantity"));
		if (!resultSet->isNull("planned_dispense_time")) data->setPlannedDispenseTime(resultSet->getString("planned_dispense_time"));
		if (!resultSet->isNull("procedure_id")) data->setProcedureId(resultSet->getString("procedure_id"));
		if (!resultSet->isNull("summary_no")) data->setSummaryNo(resultSet->getString("summary_no"));
		if (!resultSet->isNull("des_location_id")) data->setDesLocationId(resultSet->getString("des_location_id"));
		if (!resultSet->isNull("dosage_instruction")) data->setDosageInstruction(resultSet->getString("dosage_instruction"));
		if (!resultSet->isNull("method_code")) data->setMethodCode(resultSet->getString("method_code"));
		if (!resultSet->isNull("frequency_code")) data->setFrequencyCode(resultSet->getString("frequency_code"));
		if (!resultSet->isNull("dose")) data->setDose(resultSet->getDouble("dose"));
		if (!resultSet->isNull("dose_unit_code")) data->setDoseUnitCode(resultSet->getString("dose_unit_code"));
		if (!resultSet->isNull("max_unit")) data->setMaxUnit(resultSet->getDouble("max_unit"));
		if (!resultSet->isNull("part_type_code")) data->setPartTypeCode(resultSet->getString("part_type_code"));
		if (!resultSet->isNull("dispense_frequency_code")) data->setDispenseFrequencyCode(resultSet->getString("dispense_frequency_code"));
		return data;
	}
};

class MedicationSummaryLineMapper : public Mapper<MedicationSummaryLineRow>
{
public:
	MedicationSummaryLineRow mapper(ResultSet* resultSet) const override
	{
		MedicationSummaryLineRow row;
		row.dispenseId = resultSet->getString("dispense_id");
		row.patientId = resultSet->getString("patient_id");
		row.medicationId = resultSet->getString("medication_id");
		row.locationId = resultSet->getString("location_id");
		row.unitCode = resultSet->getString("unit_code");
		row.quantity = resultSet->getDouble("quantity");
		row.price = resultSet->getDouble("price");
		return row;
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

bool OrderExecuteControlDAO::existsBlockingChargeItemsForCancellation(
	const std::list<std::string>& procedureIds,
	const std::list<std::string>& serviceRequestIds)
{
	if (procedureIds.empty() && serviceRequestIds.empty())
		return false;

	SqlParams params;
	const string querySql = R"SQL(
		SELECT EXISTS (
			SELECT 1
			FROM adm_charge_item
			WHERE is_deleted = 0
				AND )SQL" +
		buildChargeRelationCondition(procedureIds, serviceRequestIds, params) +
		R"SQL(
				AND status_enum IN ('SETTLED', 'REFUNDING', 'REFUNDED')
			LIMIT 1
		);
	)SQL";

	return sqlSession->executeQueryNumerical(querySql, params) > 0;
}

int OrderExecuteControlDAO::voidPendingChargeItemsForCancellationNoTransaction(
	const std::list<std::string>& procedureIds,
	const std::list<std::string>& serviceRequestIds,
	const std::string& operatorId)
{
	if (procedureIds.empty() && serviceRequestIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE adm_charge_item
		SET
			is_deleted = 1,
			deleted_at = NOW(),
			deleted_by = ?,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE is_deleted = 0
			AND status_enum = 'PENDING_SETTLEMENT'
			AND generate_source_enum = 'order-execute'
			AND )SQL" +
		buildChargeRelationCondition(procedureIds, serviceRequestIds, params) +
		";";

	return sqlSession->executeUpdate(updateSql, params);
}

bool OrderExecuteControlDAO::existsBlockingMedicationDispensesForCancellation(const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return false;

	SqlParams params;
	const string querySql = R"SQL(
		SELECT EXISTS (
			SELECT 1
			FROM med_medication_dispense
			WHERE procedure_id IN (
		)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
			)
			AND is_deleted = 0
			AND status_enum IN ('PREPARED', 'IN_PROGRESS')
			LIMIT 1
		);
	)SQL";
	pushStringParams(params, procedureIds);

	return sqlSession->executeQueryNumerical(querySql, params) > 0;
}

std::list<PtrMedicationDispenseDO> OrderExecuteControlDAO::queryCompletedMedicationDispensesForRefund(
	const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return {};

	const string querySql = R"SQL(
		SELECT
			id,
			tenant_id,
			bus_no,
			status_enum,
			dispense_enum,
			medication_id,
			patient_id,
			encounter_id,
			location_id,
			med_req_id,
			quantity,
			unit_code,
			dispense_quantity,
			planned_dispense_time,
			procedure_id,
			summary_no,
			des_location_id,
			dosage_instruction,
			method_code,
			frequency_code,
			dose,
			dose_unit_code,
			max_unit,
			part_type_code,
			dispense_frequency_code
		FROM med_medication_dispense
		WHERE procedure_id IN (
	)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0
		AND status_enum = 'COMPLETED';
	)SQL";

	SqlParams params;
	pushStringParams(params, procedureIds);
	return sqlSession->executeQuery<PtrMedicationDispenseDO>(
		querySql,
		MedicationDispenseForCancelMapper(),
		params);
}

int OrderExecuteControlDAO::insertMedicationRefundDispensesNoTransaction(const std::list<MedicationDispenseDO>& dispenses)
{
	if (dispenses.empty())
		return 0;

	int rows = 0;
	for (const auto& dispense : dispenses)
	{
		rows += sqlSession->executeUpdate(
			R"SQL(
				INSERT INTO med_medication_dispense(
					id,
					created_at,
					created_by,
					is_deleted,
					version,
					bus_no,
					status_enum,
					status_changed_time,
					dispense_enum,
					medication_id,
					patient_id,
					encounter_id,
					location_id,
					med_req_id,
					quantity,
					unit_code,
					procedure_id,
					rec_practitioner_id
				)
				VALUES(?, ?, ?, 0, 0, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
			)SQL",
			"%s%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s",
			dispense.getId(),
			dispense.getCreatedAt(),
			dispense.getCreatedBy(),
			dispense.getBusNo(),
			dispense.getStatusEnum(),
			dispense.getStatusChangedTime(),
			dispense.getDispenseEnum(),
			dispense.getMedicationId(),
			dispense.getPatientId(),
			dispense.getEncounterId(),
			dispense.getLocationId(),
			dispense.getMedReqId(),
			dispense.getQuantity(),
			dispense.getUnitCode(),
			dispense.getProcedureId(),
			dispense.getRecPractitionerId());
	}
	return rows;
}

int OrderExecuteControlDAO::softDeleteMedicationDispensesForCancellationNoTransaction(
	const std::list<std::string>& procedureIds,
	const std::string& operatorId)
{
	if (procedureIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE med_medication_dispense
		SET
			status_enum = 'ON_HOLD',
			is_deleted = 1,
			deleted_at = NOW(),
			deleted_by = ?,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE procedure_id IN (
	)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0
		AND status_enum IN ('PREPARATION', 'SUMMARIZED');
	)SQL";
	pushStringParams(params, procedureIds);

	return sqlSession->executeUpdate(updateSql, params);
}

std::list<std::string> OrderExecuteControlDAO::listMedicationSummaryNosByProcedureIds(
	const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return {};

	const string querySql = R"SQL(
		SELECT DISTINCT summary_no
		FROM med_medication_dispense
		WHERE procedure_id IN (
	)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0
		AND summary_no IS NOT NULL
		AND summary_no <> '';
	)SQL";

	SqlParams params;
	pushStringParams(params, procedureIds);
	return sqlSession->executeQuery<std::string>(querySql, StringColumnMapper("summary_no"), params);
}

bool OrderExecuteControlDAO::existsBlockingMedicationSummaryByNos(const std::list<std::string>& summaryNos)
{
	if (summaryNos.empty())
		return false;

	const string querySql = R"SQL(
		SELECT EXISTS (
			SELECT 1
			FROM wkf_supply_request
			WHERE bus_no IN (
		)SQL" +
		buildPlaceholders(summaryNos.size()) +
		R"SQL(
			)
			AND is_deleted = 0
			AND category_enum = 'MEDICATION'
			AND status_enum NOT IN ('DRAFT', 'PENDING_APPROVAL')
			LIMIT 1
		);
	)SQL";

	SqlParams params;
	pushStringParams(params, summaryNos);
	return sqlSession->executeQueryNumerical(querySql, params) > 0;
}

std::list<MedicationSummaryLineRow> OrderExecuteControlDAO::queryMedicationSummaryLinesForRebuild(
	const std::string& summaryNo)
{
	const string querySql = R"SQL(
		SELECT
			d.id AS dispense_id,
			d.patient_id,
			d.medication_id,
			d.location_id,
			d.unit_code,
			d.quantity,
			MAX(COALESCE(ci.unit_price, 0)) AS price
		FROM med_medication_dispense d
		LEFT JOIN adm_charge_item ci
			ON ci.dispense_id = d.id
			AND ci.is_deleted = 0
			AND ci.status_enum = 'PENDING_SETTLEMENT'
		WHERE d.is_deleted = 0
			AND d.summary_no = ?
		GROUP BY
			d.id,
			d.patient_id,
			d.medication_id,
			d.location_id,
			d.unit_code,
			d.quantity
		ORDER BY d.id ASC;
	)SQL";

	return sqlSession->executeQuery<MedicationSummaryLineRow>(
		querySql,
		MedicationSummaryLineMapper(),
		"%s",
		summaryNo);
}

int OrderExecuteControlDAO::reassignMedicationSummaryNoTransaction(
	const std::string& oldSummaryNo,
	const std::string& newSummaryNo,
	const std::string& operatorId)
{
	return sqlSession->executeUpdate(
		R"SQL(
			UPDATE med_medication_dispense
			SET summary_no = ?,
				updated_at = NOW(),
				updated_by = ?,
				version = version + 1
			WHERE is_deleted = 0
				AND summary_no = ?;
		)SQL",
		"%s%s%s",
		newSummaryNo,
		operatorId,
		oldSummaryNo);
}

int OrderExecuteControlDAO::softDeleteMedicationSummarySupplyRequestsNoTransaction(
	const std::list<std::string>& summaryNos,
	const std::string& operatorId)
{
	if (summaryNos.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE wkf_supply_request
		SET
			is_deleted = 1,
			deleted_at = NOW(),
			deleted_by = ?,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE bus_no IN (
	)SQL" +
		buildPlaceholders(summaryNos.size()) +
		R"SQL(
		)
		AND is_deleted = 0
		AND category_enum = 'MEDICATION';
	)SQL";
	pushStringParams(params, summaryNos);

	return sqlSession->executeUpdate(updateSql, params);
}

int OrderExecuteControlDAO::clearMedicationSummaryNoForCancellationNoTransaction(
	const std::list<std::string>& procedureIds,
	const std::string& operatorId)
{
	if (procedureIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE med_medication_dispense
		SET
			summary_no = NULL,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE procedure_id IN (
	)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0
		AND summary_no IS NOT NULL
		AND summary_no <> '';
	)SQL";
	pushStringParams(params, procedureIds);

	return sqlSession->executeUpdate(updateSql, params);
}

int OrderExecuteControlDAO::insertMedicationSummarySupplyRequestsNoTransaction(
	const std::list<SupplyRequestDO>& supplyRequests)
{
	if (supplyRequests.empty())
		return 0;

	int rows = 0;
	for (const auto& request : supplyRequests)
	{
		rows += sqlSession->executeUpdate(
			R"SQL(
				INSERT INTO wkf_supply_request(
					id,
					created_at,
					created_by,
					is_deleted,
					version,
					bus_no,
					type_enum,
					status_enum,
					category_enum,
					patient_id,
					item_table,
					item_quantity,
					item_id,
					unit_code,
					purpose_type_enum,
					purpose_location_id,
					applicant_id,
					price,
					total_price
				)
				VALUES(?, ?, ?, 0, 0, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
			)SQL",
			"%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%d%d",
			request.getId(),
			request.getCreatedAt(),
			request.getCreatedBy(),
			request.getBusNo(),
			request.getTypeEnum(),
			request.getStatusEnum(),
			request.getCategoryEnum(),
			request.getPatientId(),
			request.getItemTable(),
			request.getItemQuantity(),
			request.getItemId(),
			request.getUnitCode(),
			request.getPurposeTypeEnum(),
			request.getPurposeLocationId(),
			request.getApplicantId(),
			request.getPrice(),
			request.getTotalPrice());
	}
	return rows;
}

bool OrderExecuteControlDAO::existsBlockingDeviceDispensesForCancellation(const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return false;

	const string querySql = R"SQL(
		SELECT EXISTS (
			SELECT 1
			FROM wkf_device_dispense
			WHERE procedure_id IN (
		)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
			)
			AND is_deleted = 0
			AND status_enum IN ('PREPARED', 'IN_PROGRESS')
			LIMIT 1
		);
	)SQL";

	SqlParams params;
	pushStringParams(params, procedureIds);
	return sqlSession->executeQueryNumerical(querySql, params) > 0;
}

int OrderExecuteControlDAO::restoreInventoryForCompletedDeviceDispensesNoTransaction(
	const std::list<std::string>& procedureIds,
	const std::string& operatorId)
{
	if (procedureIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE wkf_inventory_item i
		JOIN wkf_device_dispense d
			ON d.device_def_id = i.item_id
			AND d.location_id = i.location_id
			AND (
				(d.lot_number IS NULL AND i.lot_number IS NULL)
				OR i.lot_number = d.lot_number
			)
		SET
			i.quantity = i.quantity + COALESCE(d.dispense_quantity, d.quantity),
			i.updated_at = NOW(),
			i.updated_by = ?,
			i.version = i.version + 1
		WHERE d.procedure_id IN (
	)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
		)
		AND d.is_deleted = 0
		AND d.status_enum = 'COMPLETED'
		AND i.is_deleted = 0;
	)SQL";
	pushStringParams(params, procedureIds);

	return sqlSession->executeUpdate(updateSql, params);
}

int OrderExecuteControlDAO::softDeleteDeviceDispensesForCancellationNoTransaction(
	const std::list<std::string>& procedureIds,
	const std::string& operatorId)
{
	if (procedureIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE wkf_device_dispense
		SET
			status_enum = 'ON_HOLD',
			is_deleted = 1,
			deleted_at = NOW(),
			deleted_by = ?,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE procedure_id IN (
	)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0
		AND status_enum IN ('PREPARATION', 'COMPLETED', 'SUMMARIZED');
	)SQL";
	pushStringParams(params, procedureIds);

	return sqlSession->executeUpdate(updateSql, params);
}

std::list<std::string> OrderExecuteControlDAO::listDerivedServiceRequestIdsForCancellation(
	const std::list<std::string>& procedureIds)
{
	if (procedureIds.empty())
		return {};

	const string querySql = R"SQL(
		SELECT DISTINCT sr.id
		FROM wkf_service_request sr
		JOIN cln_procedure p
			ON p.id IN (
		)SQL" +
		buildPlaceholders(procedureIds.size()) +
		R"SQL(
			)
			AND p.is_deleted = 0
		WHERE sr.is_deleted = 0
			AND sr.generate_source_enum = 'order-execute'
			AND (
				sr.based_on_id = p.id
				OR sr.based_on_id = p.request_id
				OR sr.parent_id = p.request_id
			);
	)SQL";

	SqlParams params;
	pushStringParams(params, procedureIds);
	return sqlSession->executeQuery<std::string>(querySql, StringColumnMapper("id"), params);
}

std::list<PtrClnProcedureDO> OrderExecuteControlDAO::queryCompletedProceduresByServiceRequestIds(
	const std::list<std::string>& serviceRequestIds)
{
	if (serviceRequestIds.empty())
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
		WHERE request_id IN (
	)SQL" +
		buildPlaceholders(serviceRequestIds.size()) +
		R"SQL(
		)
		AND request_table = 'wkf_service_request'
		AND status_enum = 'COMPLETED'
		AND is_deleted = 0
		AND occurrence_slot_active = 1;
	)SQL";

	SqlParams params;
	pushStringParams(params, serviceRequestIds);
	return sqlSession->executeQuery<PtrClnProcedureDO>(
		querySql,
		ClnProcedureCompletedMapper(),
		params);
}

int OrderExecuteControlDAO::softDeleteDerivedServiceRequestsNoTransaction(
	const std::list<std::string>& serviceRequestIds,
	const std::string& operatorId)
{
	if (serviceRequestIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE wkf_service_request
		SET
			is_deleted = 1,
			deleted_at = NOW(),
			deleted_by = ?,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE id IN (
	)SQL" +
		buildPlaceholders(serviceRequestIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0;
	)SQL";
	pushStringParams(params, serviceRequestIds);

	return sqlSession->executeUpdate(updateSql, params);
}

int OrderExecuteControlDAO::softDeleteDerivedServiceRequestDetailsNoTransaction(
	const std::list<std::string>& serviceRequestIds,
	const std::string& operatorId)
{
	if (serviceRequestIds.empty())
		return 0;

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	SQLPARAMS_PUSH(params, "s", string, operatorId);
	const string updateSql = R"SQL(
		UPDATE wkf_service_request_detail
		SET
			is_deleted = 1,
			deleted_at = NOW(),
			deleted_by = ?,
			updated_at = NOW(),
			updated_by = ?,
			version = version + 1
		WHERE service_req_id IN (
	)SQL" +
		buildPlaceholders(serviceRequestIds.size()) +
		R"SQL(
		)
		AND is_deleted = 0;
	)SQL";
	pushStringParams(params, serviceRequestIds);

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
