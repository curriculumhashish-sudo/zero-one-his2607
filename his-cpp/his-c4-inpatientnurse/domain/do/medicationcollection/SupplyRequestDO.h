#pragma once
#ifndef _SUPPLYREQUESTDO_
#define _SUPPLYREQUESTDO_

#include "../DoInclude.h"

class SupplyRequestDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, originalBusNo, OriginalBusNo);
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, intentEnum, IntentEnum);
	MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
	MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
	MYSQL_SYNTHESIZE(string, summaryId, SummaryId);
	MYSQL_SYNTHESIZE(string, deliverId, DeliverId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
	MYSQL_SYNTHESIZE(double, itemQuantity, ItemQuantity);
	MYSQL_SYNTHESIZE(double, totalQuantity, TotalQuantity);
	MYSQL_SYNTHESIZE(string, itemId, ItemId);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(string, occurrenceTime, OccurrenceTime);
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
	MYSQL_SYNTHESIZE(string, supplierId, SupplierId);
	MYSQL_SYNTHESIZE(string, reason, Reason);
	MYSQL_SYNTHESIZE(string, sourceTypeEnum, SourceTypeEnum);
	MYSQL_SYNTHESIZE(string, sourceLocationId, SourceLocationId);
	MYSQL_SYNTHESIZE(string, purposeTypeEnum, PurposeTypeEnum);
	MYSQL_SYNTHESIZE(string, purposeLocationId, PurposeLocationId);
	MYSQL_SYNTHESIZE(string, approverId, ApproverId);
	MYSQL_SYNTHESIZE(string, approvalTime, ApprovalTime);
	MYSQL_SYNTHESIZE(string, applicantId, ApplicantId);
	MYSQL_SYNTHESIZE(string, applyTime, ApplyTime);
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	MYSQL_SYNTHESIZE(string, endTime, EndTime);
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	MYSQL_SYNTHESIZE(double, price, Price);
	MYSQL_SYNTHESIZE(double, totalPrice, TotalPrice);
	MYSQL_SYNTHESIZE(string, remake, Remake);
	MYSQL_SYNTHESIZE(double, profitLossPrice, ProfitLossPrice);

public:
	SupplyRequestDO() : BaseDO("wkf_supply_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD_NULLABLE("original_bus_no", "s", originalBusNo, true);
		MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("intent_enum", "s", intentEnum, true);
		MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
		MYSQL_ADD_FIELD_NULLABLE("priority_enum", "s", priorityEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("summary_id", "s", summaryId, true);
		MYSQL_ADD_FIELD_NULLABLE("deliver_id", "s", deliverId, true);
		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, true);
		MYSQL_ADD_FIELD("item_table", "s", itemTable);
		MYSQL_ADD_FIELD("item_quantity", "d", itemQuantity);
		MYSQL_ADD_FIELD_NULLABLE("total_quantity", "d", totalQuantity, true);
		MYSQL_ADD_FIELD("item_id", "s", itemId);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_time", "s", occurrenceTime, true);
		MYSQL_ADD_FIELD_NULLABLE("practitioner_id", "s", practitionerId, true);
		MYSQL_ADD_FIELD_NULLABLE("supplier_id", "s", supplierId, true);
		MYSQL_ADD_FIELD_NULLABLE("reason", "s", reason, true);
		MYSQL_ADD_FIELD_NULLABLE("source_type_enum", "s", sourceTypeEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("source_location_id", "s", sourceLocationId, true);
		MYSQL_ADD_FIELD_NULLABLE("purpose_type_enum", "s", purposeTypeEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("purpose_location_id", "s", purposeLocationId, true);
		MYSQL_ADD_FIELD_NULLABLE("approver_id", "s", approverId, true);
		MYSQL_ADD_FIELD_NULLABLE("approval_time", "s", approvalTime, true);
		MYSQL_ADD_FIELD("applicant_id", "s", applicantId);
		MYSQL_ADD_FIELD_NULLABLE("apply_time", "s", applyTime, true);
		MYSQL_ADD_FIELD_NULLABLE("start_time", "s", startTime, true);
		MYSQL_ADD_FIELD_NULLABLE("end_time", "s", endTime, true);
		MYSQL_ADD_FIELD_NULLABLE("lot_number", "s", lotNumber, true);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo, true);
		MYSQL_ADD_FIELD_NULLABLE("price", "d", price, true);
		MYSQL_ADD_FIELD_NULLABLE("total_price", "d", totalPrice, true);
		MYSQL_ADD_FIELD_NULLABLE("remake", "s", remake, true);
		MYSQL_ADD_FIELD_NULLABLE("profit_loss_price", "d", profitLossPrice, true);
	}
};

typedef std::shared_ptr<SupplyRequestDO> PtrSupplyRequestDO;

#endif
