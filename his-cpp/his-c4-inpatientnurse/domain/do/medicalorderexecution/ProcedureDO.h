#pragma once

#ifndef _CLN_PROCEDURE_DO_
#define _CLN_PROCEDURE_DO_

#include "../DoInclude.h"

class ClnProcedureDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, requestId, RequestId);
	MYSQL_SYNTHESIZE(string, requestTable, RequestTable);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, statusReasonText, StatusReasonText);
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
	MYSQL_SYNTHESIZE(string, procedureCode, ProcedureCode);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, occurrenceTime, OccurrenceTime);
	MYSQL_SYNTHESIZE(string, occurrencePeriod, OccurrencePeriod);
	MYSQL_SYNTHESIZE(string, occurrenceAge, OccurrenceAge);
	MYSQL_SYNTHESIZE(string, occurrenceText, OccurrenceText);
	MYSQL_SYNTHESIZE(string, recordedTime, RecordedTime);
	MYSQL_SYNTHESIZE(string, recorderId, RecorderId);
	MYSQL_SYNTHESIZE(bool, reportFlag, ReportFlag);
	MYSQL_SYNTHESIZE(string, reportedPatientId, ReportedPatientId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, reasonText, ReasonText);
	MYSQL_SYNTHESIZE(string, bodySiteJson, BodySiteJson);
	MYSQL_SYNTHESIZE(string, bodyStructureId, BodyStructureId);
	MYSQL_SYNTHESIZE(string, resultText, ResultText);
	MYSQL_SYNTHESIZE(string, reportJson, ReportJson);
	MYSQL_SYNTHESIZE(string, complicationJson, ComplicationJson);
	MYSQL_SYNTHESIZE(string, followUpJson, FollowUpJson);
	MYSQL_SYNTHESIZE(string, groupId, GroupId);
	MYSQL_SYNTHESIZE(string, refundId, RefundId);
	MYSQL_SYNTHESIZE(bool, occurrenceSlotActive, OccurrenceSlotActive);

public:
	ClnProcedureDO() : BaseDO("cln_procedure")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("request_id", "s", requestId);
		MYSQL_ADD_FIELD("request_table", "s", requestTable);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("status_reason_text", "s", statusReasonText, true);
		MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
		MYSQL_ADD_FIELD_NULLABLE("procedure_code", "s", procedureCode, true);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_time", "dt", occurrenceTime, true);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_period", "s", occurrencePeriod, true);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_age", "s", occurrenceAge, true);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_text", "s", occurrenceText, true);
		MYSQL_ADD_FIELD_NULLABLE("recorded_time", "dt", recordedTime, true);
		MYSQL_ADD_FIELD_NULLABLE("recorder_id", "s", recorderId, true);
		MYSQL_ADD_FIELD_NULLABLE("report_flag", "b", reportFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("reported_patient_id", "s", reportedPatientId, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("reason_text", "s", reasonText, true);
		MYSQL_ADD_FIELD_NULLABLE("body_site_json", "s", bodySiteJson, true);
		MYSQL_ADD_FIELD_NULLABLE("body_structure_id", "s", bodyStructureId, true);
		MYSQL_ADD_FIELD_NULLABLE("result_text", "s", resultText, true);
		MYSQL_ADD_FIELD_NULLABLE("report_json", "s", reportJson, true);
		MYSQL_ADD_FIELD_NULLABLE("complication_json", "s", complicationJson, true);
		MYSQL_ADD_FIELD_NULLABLE("follow_up_json", "s", followUpJson, true);
		MYSQL_ADD_FIELD_NULLABLE("group_id", "s", groupId, true);
		MYSQL_ADD_FIELD_NULLABLE("refund_id", "s", refundId, true);
		MYSQL_ADD_FIELD("occurrence_slot_active", "b", occurrenceSlotActive);
	}

	void setNotExecuteInsertFields(
		const string& procedureId,
		const string& nowTime,
		const string& operatorId,
		const string& requestId,
		const string& requestTable,
		const string& categoryEnum,
		const string& encounterId,
		const string& patientId,
		const string& occurrenceTime)
	{
		setId(procedureId);
		setCreatedAt(nowTime);
		setCreatedBy(operatorId);
		setIsDeleted(false);
		setVersion(0);
		setRequestId(requestId);
		setRequestTable(requestTable);
		setStatusEnum("STOPPED");
		setCategoryEnum(categoryEnum);
		setEncounterId(encounterId);
		setPatientId(patientId);
		setOccurrenceTime(occurrenceTime);
		setRecordedTime(nowTime);
		setRecorderId(operatorId);
		setOccurrenceSlotActive(true);
	}
};

typedef std::shared_ptr<ClnProcedureDO> PtrClnProcedureDO;

#endif // !_CLN_PROCEDURE_DO_
