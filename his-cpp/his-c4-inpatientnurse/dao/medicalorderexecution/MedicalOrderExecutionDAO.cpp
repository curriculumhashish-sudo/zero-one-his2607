/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/20 16:00:00

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
#include "MedicalOrderExecutionDAO.h"

namespace {
class ProcedureIdMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};

class ExecutionWriteContextMapper : public Mapper<ExecutionWriteContext>
{
public:
	ExecutionWriteContext mapper(ResultSet* resultSet) const override
	{
		ExecutionWriteContext row;
		row.orderType = resultSet->getString("order_type");
		row.requestTable = resultSet->getString("request_table");
		row.requestId = resultSet->getString("request_id");
		row.prescriptionNo = resultSet->getString("prescription_no");
		row.patientId = resultSet->getString("patient_id");
		row.encounterId = resultSet->getString("encounter_id");
		row.therapyEnum = resultSet->getString("therapy_enum");
		row.productTable = resultSet->getString("product_table");
		row.productId = resultSet->getString("product_id");
		row.chargeItemDefinitionId = resultSet->getString("definition_id");
		row.locationId = resultSet->getString("location_id");
		row.destinationLocationId = resultSet->getString("destination_location_id");
		row.orgId = resultSet->getString("org_id");
		row.unitCode = resultSet->getString("unit_code");
		row.dosageInstruction = resultSet->getString("dosage_instruction");
		row.methodCode = resultSet->getString("method_code");
		row.frequencyCode = resultSet->getString("frequency_code");
		row.quantity = resultSet->getDouble("quantity");
		row.unitPrice = resultSet->getDouble("unit_price");
		return row;
	}
};

class ExecutionBindingMapper : public Mapper<OrderExecutionChargeBindingDTO::Wrapper>
{
public:
	OrderExecutionChargeBindingDTO::Wrapper mapper(ResultSet* resultSet) const override
	{
		auto row = OrderExecutionChargeBindingDTO::createShared();
		row->bindType = resultSet->getString("bind_type").asStdString();
		row->bindingType = row->bindType;
		row->bindingId = resultSet->getString("binding_id").asStdString();
		row->activityId = resultSet->getString("activity_id").asStdString();
		row->deviceId = resultSet->getString("device_id").asStdString();
		row->chargeItemDefinitionId = resultSet->getString("definition_id").asStdString();
		row->name = resultSet->getString("display_name").asStdString();
		row->specificationText = resultSet->getString("specification_text").asStdString();
		row->unitPrice = resultSet->getDouble("unit_price");
		row->quantity = resultSet->getDouble("quantity");
		row->unitCode = resultSet->getString("unit_code").asStdString();
		row->priorityEnum = resultSet->getString("priority_value").asStdString();
		return row;
	}
};

class StringMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};
}

// ============================================================================
// Mapper: converts a ResultSet row to WkfServiceRequestDO.
// Required by SqlSession::executeQuery -- it calls mapper.mapper(ResultSet*),
// so we must subclass Mapper<T> instead of using a plain lambda.
// ============================================================================
class MedicalOrderExecutionRowMapper : public Mapper<PtrMedicalOrderExecutionRow>
{
public:
    PtrMedicalOrderExecutionRow mapper(ResultSet* rs) const override
    {
        auto row = std::make_shared<MedicalOrderExecutionRow>();
        auto sr = std::make_shared<WkfServiceRequestDO>();
        sr->setId(rs->getString("id"));
        sr->setBusNo(rs->getString("bus_no"));
        sr->setPrescriptionNo(rs->getString("prescription_no"));
        sr->setStatusEnum(rs->getString("status_enum"));
		sr->setIntentEnum(rs->getString("therapy_enum"));
		sr->setTherapyEnum(rs->getString("therapy_enum"));
        sr->setCategoryEnum(rs->getString("category_enum"));
        sr->setPriorityEnum(rs->getString("priority_enum"));
        sr->setPatientId(rs->getString("patient_id"));
        sr->setEncounterId(rs->getString("encounter_id"));
        sr->setAuthoredTime(rs->getString("authored_time"));
		sr->setOccurrenceStartTime(rs->getString("plan_time"));
		sr->setContentJson(rs->getString("advice_display_name"));
        sr->setRequesterId(rs->getString("requester_id"));
        sr->setPerformerId(rs->getString("performer_id"));
        sr->setPerformerCheckId(rs->getString("performer_check_id"));
        sr->setCheckTime(rs->getString("check_time"));
        sr->setLocationId(rs->getString("location_id"));
        sr->setQuantity(rs->getDouble("quantity"));
        sr->setUnitCode(rs->getString("unit_code"));
        sr->setExecutionCount(rs->getInt("execution_count"));
        sr->setRemark(rs->getString("remark"));
        sr->setEntrustContent(rs->getString("entrust_content"));
        sr->setReasonText(rs->getString("reason_text"));
        row->order = sr;
        row->orderType = rs->getString("order_type").asStdString();
		row->procedureId = rs->getString("procedure_id").asStdString();
		row->recordedTime = rs->getString("recorded_time").asStdString();
		row->specificationText = rs->getString("specification_text").asStdString();
		row->dosageDisplay = rs->getString("dosage_display").asStdString();
		row->usageDisplay = rs->getString("usage_display").asStdString();
		row->frequencyDisplay = rs->getString("frequency_display").asStdString();
		row->stopTime = rs->getString("stop_time").asStdString();
		row->requesterName = rs->getString("requester_name").asStdString();
		row->locationName = rs->getString("location_name").asStdString();
		row->tcmFlag = rs->getBoolean("tcm_flag");
		row->chineseHerbsDoseQuantity = rs->getDouble("chinese_herbs_dose_quantity");
        return row;
    }
};

static std::string buildOrderUnion()
{
    return
		"SELECT o.id,o.bus_no,o.prescription_no,COALESCE(procedure_row.status_enum,o.status_enum) status_enum,"
		"o.therapy_enum,o.category_enum,o.priority_enum,o.patient_id,o.encounter_id,o.authored_time,"
		"o.plan_time,o.advice_display_name,o.requester_id,o.performer_id,o.performer_check_id,"
		"o.check_time,o.location_id,o.quantity,o.unit_code,o.execution_count,o.remark,o.entrust_content,"
		"o.reason_text,o.order_type,COALESCE(procedure_row.id,cancel_row.id,'') procedure_id,"
		"COALESCE(procedure_row.recorded_time,cancel_row.recorded_time,'') recorded_time,o.specification_text,"
		"o.dosage_display,o.usage_display,o.frequency_display,o.stop_time,"
		"o.requester_name,o.location_name,o.tcm_flag,o.chinese_herbs_dose_quantity FROM ("
		"SELECT sr.id,sr.bus_no,sr.prescription_no,sr.status_enum,sr.therapy_enum,"
		"sr.category_enum,sr.priority_enum,sr.patient_id,sr.encounter_id,sr.authored_time,"
		"COALESCE(sr.occurrence_start_time,sr.authored_time) plan_time,"
		"COALESCE(ad.name,sr.entrust_content,sr.content_json,sr.remark,'') advice_display_name,"
		"sr.requester_id,sr.performer_id,sr.performer_check_id,sr.check_time,sr.location_id,"
        "sr.quantity,sr.unit_code,sr.execution_count,sr.remark,sr.entrust_content,"
		"sr.reason_text,'SERVICE_REQUEST' AS order_type,sr.is_deleted,"
		"COALESCE(ad.description_text,'') specification_text,"
		"CONCAT(COALESCE(CAST(sr.quantity AS CHAR),''),COALESCE(sr.unit_code,'')) dosage_display,"
		"COALESCE(sr.entrust_content,'') usage_display,COALESCE(sr.rate_code,'') frequency_display,"
		"COALESCE(sr.occurrence_end_time,'') stop_time,COALESCE(pr.name,'') requester_name,"
		"COALESCE(loc.name,'') location_name,0 tcm_flag,0 chinese_herbs_dose_quantity "
		"FROM wkf_service_request sr LEFT JOIN wkf_activity_definition ad ON ad.id=sr.activity_id AND ad.is_deleted=0 "
		"LEFT JOIN adm_practitioner pr ON pr.id=sr.requester_id AND pr.is_deleted=0 "
		"LEFT JOIN adm_location loc ON loc.id=sr.location_id AND loc.is_deleted=0 UNION ALL "
		"SELECT mr.id,mr.bus_no,mr.prescription_no,mr.status_enum,mr.therapy_enum,"
		"mr.category_enum,mr.priority_enum,mr.patient_id,mr.encounter_id,mr.req_authored_time,"
		"COALESCE(mr.effective_dose_start,mr.req_authored_time),COALESCE(md.name,mr.content_json,mr.dosage_instruction,mr.remark,''),"
		"mr.requester_id,mr.performer_id,mr.performer_check_id,mr.check_time,mr.location_id,"
        "mr.quantity,mr.unit_code,mr.execute_num,mr.remark,mr.dosage_instruction,"
		"CAST(mr.status_reason AS CHAR),'MED_REQUEST' AS order_type,mr.is_deleted,"
		"COALESCE(md.total_volume,'') specification_text,"
		"CONCAT(COALESCE(CAST(mr.dose AS CHAR),''),COALESCE(mr.dose_unit_code,'')) dosage_display,"
		"COALESCE(mr.method_code,'') usage_display,COALESCE(mr.rate_code,'') frequency_display,"
		"COALESCE(mr.effective_dose_end,'') stop_time,COALESCE(pr.name,'') requester_name,"
		"COALESCE(loc.name,'') location_name,COALESCE(mr.tcm_flag,0) tcm_flag,"
		"COALESCE(mr.chinese_herbs_dose_quantity,0) chinese_herbs_dose_quantity "
		"FROM med_medication_request mr LEFT JOIN med_medication_definition md ON md.id=mr.medication_id AND md.is_deleted=0 "
		"LEFT JOIN adm_practitioner pr ON pr.id=mr.requester_id AND pr.is_deleted=0 "
		"LEFT JOIN adm_location loc ON loc.id=mr.location_id AND loc.is_deleted=0) o "
		"LEFT JOIN cln_procedure procedure_row ON procedure_row.request_id=o.id "
		"AND procedure_row.request_table=IF(o.order_type='MED_REQUEST','med_medication_request','wkf_service_request') "
		"AND procedure_row.occurrence_time=o.plan_time AND procedure_row.occurrence_slot_active=1 "
		"AND procedure_row.is_deleted=0 "
		"LEFT JOIN cln_procedure cancel_row ON cancel_row.id=(SELECT cancelled.id "
		"FROM cln_procedure original JOIN cln_procedure cancelled "
		"ON cancelled.refund_id=original.id AND cancelled.status_enum='CANCEL' "
		"AND cancelled.is_deleted=0 WHERE original.request_id=o.id "
		"AND original.request_table=IF(o.order_type='MED_REQUEST',"
		"'med_medication_request','wkf_service_request') "
		"AND original.occurrence_time=o.plan_time AND original.is_deleted=0 "
		"ORDER BY cancelled.created_at DESC LIMIT 1) WHERE o.is_deleted=0 ";
}

// ============================================================================
// Helper: dynamically build WHERE clause from query filters.
// Only adds conditions for fields that are actually populated.
// ============================================================================
static void buildWhereClause(const MedicalOrderQuery::Wrapper& query,
                              std::string& sql, SqlParams& params)
{
    if (query->statusEnum && !query->statusEnum.getValue("").empty()) {
		sql += " AND COALESCE(procedure_row.status_enum,o.status_enum) = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
    }
    if (query->patientId && !query->patientId.getValue("").empty()) {
        sql += " AND o.patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
    }
    if (query->encounterId && !query->encounterId.getValue("").empty()) {
        sql += " AND o.encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
    }
    if (query->startDate && !query->startDate.getValue("").empty()) {
		sql += " AND o.plan_time >= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->startDate.getValue(""));
    }
    if (query->endDate && !query->endDate.getValue("").empty()) {
		sql += " AND o.plan_time <= ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->endDate.getValue(""));
    }
    if (query->locationId && !query->locationId.getValue("").empty()) {
        sql += " AND o.location_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->locationId.getValue(""));
    }
    if (query->categoryEnum && !query->categoryEnum.getValue("").empty()) {
        sql += " AND o.category_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->categoryEnum.getValue(""));
    }
    if (query->priorityEnum && !query->priorityEnum.getValue("").empty()) {
        sql += " AND o.priority_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->priorityEnum.getValue(""));
    }
	if (query->intentEnum && !query->intentEnum.getValue("").empty()) {
		sql += " AND o.therapy_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->intentEnum.getValue(""));
    }
	if (query->therapyEnum && !query->therapyEnum.getValue("").empty()) {
		sql += " AND o.therapy_enum = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->therapyEnum.getValue(""));
	}
}

// ============================================================================
// Requirement 1: query verification items
// ============================================================================
std::list<PtrMedicalOrderExecutionRow> MedicalOrderExecutionDAO::queryVerificationItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = buildOrderUnion();

    buildWhereClause(query, sql, params);
    sql += " ORDER BY o.authored_time DESC LIMIT ?, ?";

    SQLPARAMS_PUSH(params, "ull", uint64_t,
        (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
    SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));

    return sqlSession->executeQuery<PtrMedicalOrderExecutionRow>(
        sql, MedicalOrderExecutionRowMapper(), params);
}

// ============================================================================
// Count verification items
// ============================================================================
uint64_t MedicalOrderExecutionDAO::countVerificationItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string filtered = buildOrderUnion();
    buildWhereClause(query, filtered, params);
    std::string sql = "SELECT COUNT(1) FROM (" + filtered + ") c";

    return sqlSession->executeQueryNumerical(sql, params);
}

// ============================================================================
// Requirement 4: query execution items
// ============================================================================
std::list<PtrMedicalOrderExecutionRow> MedicalOrderExecutionDAO::queryExecutionItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = buildOrderUnion();

    buildWhereClause(query, sql, params);
    sql += " ORDER BY o.authored_time DESC LIMIT ?, ?";

    SQLPARAMS_PUSH(params, "ull", uint64_t,
        (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
    SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));

    return sqlSession->executeQuery<PtrMedicalOrderExecutionRow>(
        sql, MedicalOrderExecutionRowMapper(), params);
}

std::list<PtrMedicalOrderExecutionRow>
MedicalOrderExecutionDAO::queryExecutionWorkbenchItems(
	const OrderExecutionWorkbenchQueryDTO::Wrapper& query)
{
	SqlParams params;
	std::string sql = buildOrderUnion();
	sql += " AND o.encounter_id IN (";
	for (std::size_t index = 0; index < query->encounterIds->size(); ++index) {
		if (index > 0) sql += ',';
		sql += '?';
	}
	for (const auto& encounterId : *query->encounterIds)
		SQLPARAMS_PUSH(params, "s", std::string, encounterId.getValue(""));
	sql += ") AND o.plan_time<=?";
	SQLPARAMS_PUSH(params, "dt", std::string, query->executionDeadline.getValue(""));
	if (query->therapyType != "ALL") {
		sql += " AND o.therapy_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->therapyType.getValue(""));
	}
	if (query->medicineCategory == "TCM") sql += " AND o.tcm_flag=1";
	else sql += " AND o.tcm_flag=0";
	const std::string status = query->executeStatus.getValue("");
	if (status == "PENDING") sql += " AND procedure_row.id IS NULL AND o.status_enum='VERIFIED'";
	else if (status == "EXECUTED") sql += " AND procedure_row.status_enum='COMPLETED'";
	else if (status == "SKIPPED") sql += " AND procedure_row.status_enum='STOPPED'";
	else if (status == "CANCELLED") sql +=
		" AND EXISTS(SELECT 1 FROM cln_procedure original "
		"JOIN cln_procedure cancelled ON cancelled.refund_id=original.id "
		"AND cancelled.status_enum='CANCEL' AND cancelled.is_deleted=0 "
		"WHERE original.request_id=o.id AND original.request_table="
		"IF(o.order_type='MED_REQUEST','med_medication_request','wkf_service_request') "
		"AND original.occurrence_time=o.plan_time AND original.is_deleted=0)";
	sql += " ORDER BY o.encounter_id,o.plan_time,o.id";
	return sqlSession->executeQuery<PtrMedicalOrderExecutionRow>(
		sql, MedicalOrderExecutionRowMapper(), params);
}

bool MedicalOrderExecutionDAO::validExecutionAccount(
	const std::string& accountId, const std::string& encounterId)
{
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM adm_account WHERE id=? AND encounter_id=? "
		"AND is_deleted=0 AND COALESCE(status_enum,'ACTIVE')='ACTIVE'",
		"%s%s", accountId, encounterId) == 1;
}

std::list<OrderExecutionChargeBindingDTO::Wrapper>
MedicalOrderExecutionDAO::selectExecutionBindings(
	const std::string& orderType, const std::string& requestId)
{
	std::string itemTable;
	std::string itemId;
	if (orderType == "MED_REQUEST")
	{
		itemTable = "ter_concept";
		itemId = sqlSession->executeQueryOne<std::string>(
			"SELECT method_code FROM med_medication_request "
			"WHERE id=? AND is_deleted=0 LIMIT 1",
			StringMapper(), "%s", requestId);
	}
	else if (orderType == "SERVICE_REQUEST")
	{
		itemTable = "wkf_activity_definition";
		itemId = sqlSession->executeQueryOne<std::string>(
			"SELECT activity_id FROM wkf_service_request "
			"WHERE id=? AND is_deleted=0 LIMIT 1",
			StringMapper(), "%s", requestId);
	}
	if (itemId.empty()) return {};
	return sqlSession->executeQuery<OrderExecutionChargeBindingDTO::Wrapper>(R"SQL(
		SELECT t.type_code bind_type,t.id binding_id,
			IF(t.dev_act_table='wkf_activity_definition',t.dev_act_id,'') activity_id,
			IF(t.dev_act_table='adm_device_definition',t.dev_act_id,'') device_id,
			COALESCE(price_line.id,'') definition_id,
			COALESCE(activity.name,device.name,'') display_name,
			COALESCE(activity.description_text,device.model_number,device.description,'') specification_text,
			COALESCE(price_line.price_value,0) unit_price,t.quantity,t.unit_code,
			CAST(COALESCE(price_line.priority,0) AS CHAR) priority_value
		FROM cus_service_device_template t
		LEFT JOIN wkf_activity_definition activity
			ON t.dev_act_table='wkf_activity_definition' AND activity.id=t.dev_act_id
			AND activity.is_deleted=0 AND activity.status_enum='ACTIVE'
		LEFT JOIN adm_device_definition device
			ON t.dev_act_table='adm_device_definition' AND device.id=t.dev_act_id
			AND device.is_deleted=0 AND device.status_enum='ACTIVE' AND device.active_flag=1
		LEFT JOIN adm_charge_item_definition price_line
			ON price_line.instance_table=t.dev_act_table AND price_line.instance_id=t.dev_act_id
			AND price_line.is_deleted=0 AND price_line.status_enum='ACTIVE'
			AND price_line.id=(
				SELECT selected_price.id FROM adm_charge_item_definition selected_price
				WHERE selected_price.instance_table=t.dev_act_table
					AND selected_price.instance_id=t.dev_act_id
					AND selected_price.is_deleted=0
					AND selected_price.status_enum='ACTIVE'
				ORDER BY selected_price.priority,selected_price.id LIMIT 1)
		WHERE t.is_deleted=0 AND t.status_enum='ACTIVE'
			AND t.item_table=? AND t.item_id=?
		ORDER BY COALESCE(price_line.priority,2147483647),t.id
	)SQL", ExecutionBindingMapper(), "%s%s", itemTable, itemId);
}

bool MedicalOrderExecutionDAO::loadExecutionWriteContext(
	const std::string& orderType, const std::string& requestId,
	const std::string& encounterId, const std::string& accountId,
	ExecutionWriteContext& context)
{
	std::string sql;
	if (orderType == "MED_REQUEST")
	{
		sql = R"SQL(
			SELECT 'MED_REQUEST' order_type,'med_medication_request' request_table,
				mr.id request_id,COALESCE(mr.prescription_no,'') prescription_no,
				mr.patient_id,mr.encounter_id,mr.therapy_enum,
				'med_medication_definition' product_table,mr.medication_id product_id,
				COALESCE(price_line.id,'') definition_id,
				COALESCE(CAST(mr.perform_location AS CHAR),location_rule.def_location_id,
					md.location_id,mr.location_id,'') location_id,
				COALESCE((SELECT COALESCE(ward.id,house.id,bed.id)
					FROM adm_encounter_location encounter_location
					JOIN adm_location bed ON bed.id=encounter_location.location_id
						AND bed.is_deleted=0
					LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0
					LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0
					WHERE encounter_location.encounter_id=mr.encounter_id
						AND encounter_location.form_enum='BED'
						AND encounter_location.status_enum='ACTIVE'
						AND encounter_location.is_deleted=0 LIMIT 1),mr.location_id,'')
					destination_location_id,
				COALESCE(mr.org_id,md.org_id,'') org_id,mr.unit_code,
				COALESCE(mr.dosage_instruction,'') dosage_instruction,
				COALESCE(mr.method_code,'') method_code,
				COALESCE(mr.rate_code,'') frequency_code,mr.quantity,
				COALESCE(price_line.price_value,0) unit_price
			FROM med_medication_request mr
			JOIN adm_encounter encounter_row ON encounter_row.id=mr.encounter_id
				AND encounter_row.patient_id=mr.patient_id AND encounter_row.is_deleted=0
				AND encounter_row.status_enum='IN_PROGRESS'
			JOIN adm_account account_row ON account_row.id=? AND account_row.encounter_id=mr.encounter_id
				AND account_row.patient_id=mr.patient_id AND account_row.is_deleted=0
				AND account_row.status_enum='ACTIVE'
			JOIN med_medication_definition md ON md.id=mr.medication_id
				AND md.is_deleted=0 AND md.status_enum='ACTIVE' AND md.active_flag=1
			LEFT JOIN cus_service_location_template location_rule
				ON location_rule.item_table='med_medication_definition'
				AND (location_rule.item_definition_id IS NULL
					OR location_rule.item_definition_id=mr.medication_id)
				AND location_rule.organization_id=COALESCE(mr.org_id,md.org_id)
				AND location_rule.is_deleted=0
			LEFT JOIN adm_charge_item_definition price_line
				ON price_line.instance_table='med_medication_definition'
				AND price_line.instance_id=mr.medication_id AND price_line.is_deleted=0
				AND price_line.status_enum='ACTIVE'
			WHERE mr.id=? AND mr.encounter_id=? AND mr.status_enum='VERIFIED'
				AND mr.is_deleted=0
			ORDER BY CASE WHEN location_rule.item_definition_id=mr.medication_id THEN 0 ELSE 1 END,
				location_rule.priority,price_line.priority,price_line.id LIMIT 1
		)SQL";
	}
	else if (orderType == "SERVICE_REQUEST")
	{
		sql = R"SQL(
			SELECT 'SERVICE_REQUEST' order_type,'wkf_service_request' request_table,
				sr.id request_id,COALESCE(sr.prescription_no,'') prescription_no,
				sr.patient_id,sr.encounter_id,sr.therapy_enum,
				'wkf_activity_definition' product_table,sr.activity_id product_id,
				COALESCE(price_line.id,'') definition_id,COALESCE(sr.location_id,'') location_id,
				COALESCE((SELECT COALESCE(ward.id,house.id,bed.id)
					FROM adm_encounter_location encounter_location
					JOIN adm_location bed ON bed.id=encounter_location.location_id
						AND bed.is_deleted=0
					LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0
					LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0
					WHERE encounter_location.encounter_id=sr.encounter_id
						AND encounter_location.form_enum='BED'
						AND encounter_location.status_enum='ACTIVE'
						AND encounter_location.is_deleted=0 LIMIT 1),sr.location_id,'')
					destination_location_id,
				COALESCE(sr.org_id,activity.org_id,'') org_id,sr.unit_code,
				COALESCE(sr.entrust_content,'') dosage_instruction,'' method_code,
				COALESCE(sr.rate_code,'') frequency_code,sr.quantity,
				COALESCE(price_line.price_value,0) unit_price
			FROM wkf_service_request sr
			JOIN adm_encounter encounter_row ON encounter_row.id=sr.encounter_id
				AND encounter_row.patient_id=sr.patient_id AND encounter_row.is_deleted=0
				AND encounter_row.status_enum='IN_PROGRESS'
			JOIN adm_account account_row ON account_row.id=? AND account_row.encounter_id=sr.encounter_id
				AND account_row.patient_id=sr.patient_id AND account_row.is_deleted=0
				AND account_row.status_enum='ACTIVE'
			JOIN wkf_activity_definition activity ON activity.id=sr.activity_id
				AND activity.is_deleted=0 AND activity.status_enum='ACTIVE'
			LEFT JOIN adm_charge_item_definition price_line
				ON price_line.instance_table='wkf_activity_definition'
				AND price_line.instance_id=sr.activity_id AND price_line.is_deleted=0
				AND price_line.status_enum='ACTIVE'
			WHERE sr.id=? AND sr.encounter_id=? AND sr.status_enum='VERIFIED'
				AND sr.is_deleted=0
			ORDER BY price_line.priority,price_line.id LIMIT 1
		)SQL";
	}
	else return false;
	auto rows = sqlSession->executeQuery<ExecutionWriteContext>(
		sql, ExecutionWriteContextMapper(), "%s%s%s", accountId, requestId, encounterId);
	if (rows.size() != 1) return false;
	context = rows.front();
	return context.quantity > 0 && !context.unitCode.empty()
		&& !context.productId.empty() && !context.chargeItemDefinitionId.empty()
		&& (context.orderType != "MED_REQUEST" || !context.locationId.empty());
}

bool MedicalOrderExecutionDAO::canReserveMedication(
	const ExecutionWriteContext& context)
{
	if (context.orderType != "MED_REQUEST") return true;
	// Serialize availability checks for the same medication and store location.
	sqlSession->executeQuery<std::string>(R"SQL(
		SELECT id FROM wkf_inventory_item
		WHERE item_table='med_medication_definition' AND item_id=?
			AND location_id=? AND is_deleted=0 AND inventory_status_enum='ACTIVE'
		ORDER BY id FOR UPDATE
	)SQL", StringMapper(), "%s%s", context.productId, context.locationId);
	return sqlSession->executeQueryNumerical(R"SQL(
		SELECT CASE WHEN COALESCE(SUM(i.quantity),0)-COALESCE(reserved.quantity,0)>=? THEN 1 ELSE 0 END
		FROM wkf_inventory_item i
		LEFT JOIN (
			SELECT medication_id,location_id,SUM(quantity) quantity
			FROM med_medication_dispense
			WHERE is_deleted=0 AND status_enum IN('PREPARATION','SUMMARIZED')
			GROUP BY medication_id,location_id
		) reserved ON reserved.medication_id=i.item_id AND reserved.location_id=i.location_id
		WHERE i.item_table='med_medication_definition' AND i.item_id=?
			AND i.location_id=? AND i.is_deleted=0 AND i.inventory_status_enum='ACTIVE'
		GROUP BY reserved.quantity
	)SQL", "%d%s%s", context.quantity, context.productId, context.locationId) == 1;
}

bool MedicalOrderExecutionDAO::canCoverExecutionAmount(
	const std::string& accountId, double amount)
{
	sqlSession->executeQuery<std::string>(
		"SELECT id FROM adm_account WHERE id=? AND is_deleted=0 FOR UPDATE",
		StringMapper(), "%s", accountId);
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM adm_account WHERE id=? AND is_deleted=0 "
		"AND status_enum='ACTIVE' AND COALESCE(balance,0)>=?",
		"%s%d", accountId, amount) == 1;
}

int MedicalOrderExecutionDAO::insertProcedurePerformer(
	const std::string& id, const std::string& procedureId,
	const std::string& performerId, const std::string& actualTime)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO cln_procedure_performer(
			id,created_at,created_by,is_deleted,version,procedure_id,
			function_code,start_time,end_time,practitioner_id)
		VALUES(?,NOW(),?,0,0,?,'PRIMARY',?,NOW(),?)
	)SQL", "%s%s%s%s%s", id, performerId, procedureId, actualTime, performerId);
}

int MedicalOrderExecutionDAO::insertExecutionDispense(
	const std::string& id, const std::string& procedureId,
	const ExecutionWriteContext& context, const std::string& planTime,
	const std::string& operatorId)
{
	if (context.orderType != "MED_REQUEST") return 0;
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, id);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, "MDP" + id);
	SQLPARAMS_PUSH(params, "s", std::string, context.productId);
	SQLPARAMS_PUSH(params, "s", std::string, context.patientId);
	SQLPARAMS_PUSH(params, "s", std::string, context.encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, context.locationId);
	SQLPARAMS_PUSH(params, "s", std::string, context.destinationLocationId);
	SQLPARAMS_PUSH(params, "s", std::string, context.requestId);
	SQLPARAMS_PUSH(params, "d", double, context.quantity);
	SQLPARAMS_PUSH(params, "s", std::string, context.unitCode);
	SQLPARAMS_PUSH(params, "dt", std::string, planTime);
	SQLPARAMS_PUSH(params, "s", std::string, procedureId);
	SQLPARAMS_PUSH(params, "s", std::string, context.dosageInstruction);
	SQLPARAMS_PUSH(params, "s", std::string, context.methodCode);
	SQLPARAMS_PUSH(params, "s", std::string, context.frequencyCode);
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO med_medication_dispense(
			id,created_at,created_by,is_deleted,version,bus_no,status_enum,
			status_changed_time,dispense_enum,medication_id,patient_id,encounter_id,
			location_id,des_location_id,med_req_id,quantity,unit_code,planned_dispense_time,procedure_id,
			dosage_instruction,method_code,frequency_code)
		VALUES(?,NOW(),?,0,0,?,'PREPARATION',NOW(),'INPATIENT',?,?,?,?,?,?,?,?,?,?,?,?,?)
	)SQL", params);
}

int MedicalOrderExecutionDAO::insertExecutionCharge(
	const std::string& id, const std::string& procedureId,
	const std::string& dispenseId, const ExecutionWriteContext& context,
	const std::string& accountId, const std::string& occurrenceTime,
	const std::string& operatorId)
{
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, id);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, context.patientId);
	SQLPARAMS_PUSH(params, "s", std::string, context.encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, context.requestTable);
	SQLPARAMS_PUSH(params, "s", std::string, context.requestId);
	SQLPARAMS_PUSH(params, "s", std::string, accountId);
	SQLPARAMS_PUSH(params, "s", std::string, context.chargeItemDefinitionId);
	SQLPARAMS_PUSH(params, "d", double, context.unitPrice * context.quantity);
	SQLPARAMS_PUSH(params, "dt", std::string, occurrenceTime);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	SQLPARAMS_PUSH(params, "d", double, context.quantity);
	SQLPARAMS_PUSH(params, "s", std::string, context.unitCode);
	SQLPARAMS_PUSH(params, "d", double, context.unitPrice);
	SQLPARAMS_PUSH(params, "s", std::string, context.productTable);
	SQLPARAMS_PUSH(params, "s", std::string, context.productId);
	SQLPARAMS_PUSH(params, "s", std::string, procedureId);
	SQLPARAMS_PUSH(params, "s", std::string,
		context.orderType == "MED_REQUEST" ? "med_medication_dispense" : "");
	SQLPARAMS_PUSH(params, "s", std::string, dispenseId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO adm_charge_item(
			id,created_at,created_by,is_deleted,version,generate_source_enum,context_enum,
			patient_id,encounter_id,service_table,service_id,account_id,definition_id,
			status_enum,total_price,occurrence_time,performer_id,performing_org_id,
			requesting_org_id,cost_org_id,quantity_value,quantity_unit,unit_price,
			product_table,product_id,procedure_id,dispense_table,dispense_id,org_id)
		VALUES(?,NOW(),?,0,0,'ORDER_EXECUTE','INPATIENT',?,?,?,?,?,?,
			'PENDING_SETTLEMENT',?,?,?,?,?,?,?,?,?,?,?,?,NULLIF(?,''),NULLIF(?,''),?)
	)SQL", params);
}

int MedicalOrderExecutionDAO::insertExecutionBindingCharge(
	const std::string& id, const std::string& procedureId,
	const ExecutionWriteContext& context,
	const OrderExecutionChargeBindingDTO::Wrapper& binding,
	const std::string& accountId, const std::string& occurrenceTime,
	const std::string& operatorId)
{
	if (!binding || !binding->chargeItemDefinitionId
		|| binding->chargeItemDefinitionId->empty()
		|| binding->quantity.getValue(0) <= 0) return 0;
	const auto productTable = binding->activityId && !binding->activityId->empty()
		? "wkf_activity_definition" : "adm_device_definition";
	const auto productId = productTable == std::string("wkf_activity_definition")
		? binding->activityId.getValue("") : binding->deviceId.getValue("");
	if (productId.empty()) return 0;
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, id);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, context.patientId);
	SQLPARAMS_PUSH(params, "s", std::string, context.encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, context.requestTable);
	SQLPARAMS_PUSH(params, "s", std::string, context.requestId);
	SQLPARAMS_PUSH(params, "s", std::string, accountId);
	SQLPARAMS_PUSH(params, "s", std::string, binding->chargeItemDefinitionId.getValue(""));
	SQLPARAMS_PUSH(params, "d", double,
		binding->unitPrice.getValue(0) * binding->quantity.getValue(0));
	SQLPARAMS_PUSH(params, "dt", std::string, occurrenceTime);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	SQLPARAMS_PUSH(params, "d", double, binding->quantity.getValue(0));
	SQLPARAMS_PUSH(params, "s", std::string, binding->unitCode.getValue(""));
	SQLPARAMS_PUSH(params, "d", double, binding->unitPrice.getValue(0));
	SQLPARAMS_PUSH(params, "s", std::string, productTable);
	SQLPARAMS_PUSH(params, "s", std::string, productId);
	SQLPARAMS_PUSH(params, "s", std::string, procedureId);
	SQLPARAMS_PUSH(params, "s", std::string, context.orgId);
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO adm_charge_item(
			id,created_at,created_by,is_deleted,version,generate_source_enum,context_enum,
			patient_id,encounter_id,service_table,service_id,account_id,definition_id,
			status_enum,total_price,occurrence_time,performer_id,performing_org_id,
			requesting_org_id,cost_org_id,quantity_value,quantity_unit,unit_price,
			product_table,product_id,procedure_id,org_id)
		VALUES(?,NOW(),?,0,0,'ORDER_EXECUTE','INPATIENT',?,?,?,?,?,?,
			'PENDING_SETTLEMENT',?,?,?,?,?,?,?,?,?,?,?,?,?)
	)SQL", params);
}

std::list<std::string> MedicalOrderExecutionDAO::selectExecutionIdempotencySlots(
	const std::string& idempotencyKey)
{
	return sqlSession->executeQuery<std::string>(R"SQL(
		SELECT changes FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_ORDER_EXECUTION'
			AND entity_id=? AND operation_type='EXECUTE'
		ORDER BY created_at DESC LIMIT 1
	)SQL", StringMapper(), "%s", idempotencyKey);
}

int MedicalOrderExecutionDAO::insertExecutionIdempotency(
	const std::string& auditId, const std::string& idempotencyKey,
	const std::string& operatorId, const std::string& requestFingerprint,
	const std::string& slotMappings)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO whale_audit_logs(
			id,entity_type,entity_id,operation_type,user_id,changes,created_at)
		VALUES(?,'NURSE_STATION_ORDER_EXECUTION',?,'EXECUTE',?,?,NOW())
	)SQL", "%s%s%s%s", auditId, idempotencyKey, operatorId,
		requestFingerprint + "\n--RESULT--\n" + slotMappings);
}

// ============================================================================
// Count execution items
// ============================================================================
uint64_t MedicalOrderExecutionDAO::countExecutionItems(
    const MedicalOrderQuery::Wrapper& query)
{
    SqlParams params;
    std::string filtered = buildOrderUnion();
    buildWhereClause(query, filtered, params);
    std::string sql = "SELECT COUNT(1) FROM (" + filtered + ") c";

    return sqlSession->executeQueryNumerical(sql, params);
}

// ============================================================================
// Requirement 2: verify pass
// ============================================================================
int MedicalOrderExecutionDAO::verifyPass(
    const std::string& serviceRequestId, const std::string& verifierId)
{
    std::string sql =
        "UPDATE wkf_service_request SET "
        "status_enum = 'VERIFIED', "
        "performer_check_id = ?, "
        "check_time = NOW() "
        "WHERE id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, verifierId);
    SQLPARAMS_PUSH(params, "s", std::string, serviceRequestId);

    return sqlSession->executeUpdate(sql, params);
}

// ============================================================================
// Requirement 3: return order
// ============================================================================
int MedicalOrderExecutionDAO::returnOrder(
    const std::string& serviceRequestId, const std::string& returnReason)
{
    std::string sql =
        "UPDATE wkf_service_request SET "
        "status_enum = 'DRAFT', "
        "reason_text = ? "
        "WHERE id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, returnReason);
    SQLPARAMS_PUSH(params, "s", std::string, serviceRequestId);

    return sqlSession->executeUpdate(sql, params);
}

// ============================================================================
// Requirement 5: start execution
// ============================================================================
int MedicalOrderExecutionDAO::startExecution(
    const std::string& serviceRequestId, const std::string& performerId)
{
    std::string sql =
        "UPDATE wkf_service_request SET "
		"status_enum = 'IN_PROGRESS', "
        "performer_id = ?, "
        "execution_count = COALESCE(execution_count, 0) + 1 "
        "WHERE id = ? AND status_enum = 'VERIFIED' AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, performerId);
    SQLPARAMS_PUSH(params, "s", std::string, serviceRequestId);

    return sqlSession->executeUpdate(sql, params);
}

int MedicalOrderExecutionDAO::insertCompletedProcedure(
    const std::string& procedureId,
    const std::string& orderType,
    const std::string& requestId,
	const std::string& encounterId,
	const std::string& planTime,
	const std::string& performerId,
	const std::string& recordedTime)
{
    const bool medication = orderType == "MED_REQUEST";
    if (!medication && orderType != "SERVICE_REQUEST") return 0;
    const std::string table = medication ? "med_medication_request" : "wkf_service_request";
    const std::string requestTable = medication ? "med_medication_request" : "wkf_service_request";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, procedureId);
    SQLPARAMS_PUSH(params, "s", std::string, performerId);
    SQLPARAMS_PUSH(params, "s", std::string, requestTable);
	SQLPARAMS_PUSH(params, "dt", std::string, planTime);
	SQLPARAMS_PUSH(params, "dt", std::string, recordedTime);
    SQLPARAMS_PUSH(params, "s", std::string, performerId);
    SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, requestTable);
	SQLPARAMS_PUSH(params, "dt", std::string, planTime);
    return sqlSession->executeUpdate(
        "INSERT INTO cln_procedure(id,created_at,created_by,is_deleted,version,"
        "request_id,request_table,status_enum,category_enum,encounter_id,patient_id,"
		"occurrence_time,recorded_time,recorder_id,org_id,location_id,occurrence_slot_active) "
		"SELECT ?,NOW(),?,0,0,r.id,?,'COMPLETED',COALESCE(r.category_enum,'ORDER'),"
		"r.encounter_id,r.patient_id,?,COALESCE(NULLIF(?,''),NOW()),?,"
		"r.org_id,r.location_id,1 FROM " + table + " r "
		"WHERE r.id=? AND r.encounter_id=? AND r.status_enum='VERIFIED' AND r.is_deleted=0 "
		"AND NOT EXISTS(SELECT 1 FROM cln_procedure occupied WHERE occupied.request_id=r.id "
		"AND occupied.request_table=? AND occupied.occurrence_time=? "
		"AND occupied.occurrence_slot_active=1 AND occupied.is_deleted=0)", params);
}

std::string MedicalOrderExecutionDAO::selectActiveCompletedProcedureId(
	const std::string& orderType,const std::string& requestId,
	const std::string& encounterId,const std::string& planTime)
{
	const std::string requestTable=orderType=="MED_REQUEST"?"med_medication_request":
		(orderType=="SERVICE_REQUEST"?"wkf_service_request":"");
	if(requestTable.empty())return "";
	return sqlSession->executeQueryOne<std::string>(R"SQL(
		SELECT id FROM cln_procedure
		WHERE request_id=? AND request_table=? AND encounter_id=?
			AND occurrence_time=? AND occurrence_slot_active=1 AND is_deleted=0
			AND status_enum='COMPLETED'
		ORDER BY created_at DESC LIMIT 1
	)SQL",ProcedureIdMapper(),"%s%s%s%s",requestId,requestTable,encounterId,planTime);
}

int MedicalOrderExecutionDAO::completeOrder(
    const std::string& orderType,
    const std::string& requestId,
	const std::string& encounterId,
    const std::string& performerId)
{
    const bool medication = orderType == "MED_REQUEST";
    if (!medication && orderType != "SERVICE_REQUEST") return 0;
    const std::string table = medication ? "med_medication_request" : "wkf_service_request";
    const std::string countColumn = medication ? "execute_num" : "execution_count";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, performerId);
    SQLPARAMS_PUSH(params, "s", std::string, requestId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
    return sqlSession->executeUpdate(
        "UPDATE " + table + " SET status_enum=IF(therapy_enum='TEMPORARY','COMPLETED',status_enum),performer_id=?," +
        countColumn + "=COALESCE(" + countColumn + ",0)+1,updated_at=NOW(),version=version+1 "
		"WHERE id=? AND encounter_id=? AND status_enum='VERIFIED' AND is_deleted=0", params);
}

// ============================================================================
// Requirement 6: complete execution
// ============================================================================
int MedicalOrderExecutionDAO::completeExecution(
    const std::string& serviceRequestId, const std::string& performerId)
{
    std::string sql =
        "UPDATE wkf_service_request SET "
		"status_enum = 'COMPLETED', "
        "performer_id = ? "
        "WHERE id = ? AND status_enum = 'IN_PROGRESS' AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, performerId);
    SQLPARAMS_PUSH(params, "s", std::string, serviceRequestId);

    return sqlSession->executeUpdate(sql, params);
}
