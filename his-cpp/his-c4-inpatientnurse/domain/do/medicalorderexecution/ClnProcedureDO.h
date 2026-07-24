#pragma once

#ifndef _CLN_PROCEDURE_DO_
#define _CLN_PROCEDURE_DO_

#include "../DoInclude.h"

class ClnProcedureDO : public BaseDO
{
	// 执行记录主键，取消执行时 procedureId 就是这个字段
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户ID
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间，插入 STOPPED/CANCEL 记录时需要写入
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人，插入执行管控记录时通常取当前操作者
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删除标记，查询有效执行记录时必须过滤为 false
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号，后续更新原 COMPLETED 记录时可能使用
	MYSQL_SYNTHESIZE(int, version, Version);
	// 原医嘱ID，关联 med_medication_request 或 wkf_service_request
	MYSQL_SYNTHESIZE(string, requestId, RequestId);
	// 原医嘱来源表，用于区分药品医嘱和诊疗医嘱
	MYSQL_SYNTHESIZE(string, requestTable, RequestTable);
	// 执行状态：COMPLETED 已执行，STOPPED 不执行，CANCEL 取消执行
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// 状态原因文本，不执行原因或取消执行原因可写入这里
	MYSQL_SYNTHESIZE(string, statusReasonText, StatusReasonText);
	// 医嘱内部分类，从原医嘱表带入 cln_procedure
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
	MYSQL_SYNTHESIZE(string, procedureCode, ProcedureCode);
	// 住院就诊ID，用于权限范围、费用、执行记录关联
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// 患者ID，用于执行记录和后续费用/药品回退关联
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// 计划执行时间点，不执行 pending 净态校验的关键字段
	MYSQL_SYNTHESIZE(string, occurrenceTime, OccurrenceTime);
	MYSQL_SYNTHESIZE(string, occurrencePeriod, OccurrencePeriod);
	MYSQL_SYNTHESIZE(string, occurrenceAge, OccurrenceAge);
	MYSQL_SYNTHESIZE(string, occurrenceText, OccurrenceText);
	// 实际登记时间，插入 STOPPED/CANCEL 时可记录当前时间
	MYSQL_SYNTHESIZE(string, recordedTime, RecordedTime);
	// 登记人ID，通常取当前操作者
	MYSQL_SYNTHESIZE(string, recorderId, RecorderId);
	MYSQL_SYNTHESIZE(bool, reportFlag, ReportFlag);
	MYSQL_SYNTHESIZE(string, reportedPatientId, ReportedPatientId);
	// 执行机构ID
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	// 执行位置/病区/科室ID
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, reasonText, ReasonText);
	MYSQL_SYNTHESIZE(string, bodySiteJson, BodySiteJson);
	MYSQL_SYNTHESIZE(string, bodyStructureId, BodyStructureId);
	MYSQL_SYNTHESIZE(string, resultText, ResultText);
	MYSQL_SYNTHESIZE(string, reportJson, ReportJson);
	MYSQL_SYNTHESIZE(string, complicationJson, ComplicationJson);
	MYSQL_SYNTHESIZE(string, followUpJson, FollowUpJson);
	MYSQL_SYNTHESIZE(string, groupId, GroupId);
	// CANCEL 对冲记录指向原 COMPLETED 执行记录
	MYSQL_SYNTHESIZE(string, refundId, RefundId);
	// 计划点占用标记，pending 净态校验和取消执行释放占用都依赖它
	MYSQL_SYNTHESIZE(bool, occurrenceSlotActive, OccurrenceSlotActive);

public:
	ClnProcedureDO() : BaseDO("cln_procedure")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("request_id", "s", requestId, false);
		MYSQL_ADD_FIELD_NULLABLE("request_table", "s", requestTable, false);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("status_reason_text", "s", statusReasonText, true);
		MYSQL_ADD_FIELD_NULLABLE("category_enum", "s", categoryEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("procedure_code", "s", procedureCode, true);
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
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
		MYSQL_ADD_FIELD_NULLABLE("occurrence_slot_active", "b", occurrenceSlotActive, false);
	}

	// 设置登记不执行医嘱时需要插入的字段
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
