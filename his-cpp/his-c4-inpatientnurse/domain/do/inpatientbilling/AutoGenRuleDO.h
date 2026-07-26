#pragma once
#ifndef _INPATIENT_CHARGE_RECORD_DO_
#define _INPATIENT_CHARGE_RECORD_DO_

#include "../DoInclude.h"

// ===== cus_charge_item_auto_gen =====
class AutoGenRuleDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, ruleName, RuleName);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, triggerSourceTable, TriggerSourceTable);
	MYSQL_SYNTHESIZE(string, triggerSourceId, TriggerSourceId);
	MYSQL_SYNTHESIZE(string, chargeDefinitionId, ChargeDefinitionId);
	MYSQL_SYNTHESIZE(string, performingOrgId, PerformingOrgId);
	MYSQL_SYNTHESIZE(double, quantityValue, QuantityValue);
	MYSQL_SYNTHESIZE(int, priority, Priority);
	MYSQL_SYNTHESIZE(string, remark, Remark);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);

public:
	AutoGenRuleDO() : BaseDO("cus_charge_item_auto_gen")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("rule_name", "s", ruleName);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("trigger_source_table", "s", triggerSourceTable);
		MYSQL_ADD_FIELD("trigger_source_id", "s", triggerSourceId);
		MYSQL_ADD_FIELD("charge_definition_id", "s", chargeDefinitionId);
		MYSQL_ADD_FIELD_NULLABLE("performing_org_id", "s", performingOrgId);
		MYSQL_ADD_FIELD("quantity_value", "d", quantityValue);
		MYSQL_ADD_FIELD("priority", "i", priority);
		MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
	}
};

typedef std::shared_ptr<AutoGenRuleDO> PtrAutoGenRuleDO;

// ===== inpatient_charge_record =====
class InpatientChargeRecordDO : public BaseDO
{
	MYSQL_SYNTHESIZE(long long, id, Id);
	MYSQL_SYNTHESIZE(string, chargeNo, ChargeNo);
	MYSQL_SYNTHESIZE(string, inpNo, InpNo);
	MYSQL_SYNTHESIZE(string, orderId, OrderId);
	MYSQL_SYNTHESIZE(string, itemCode, ItemCode);
	MYSQL_SYNTHESIZE(string, itemName, ItemName);
	MYSQL_SYNTHESIZE(string, spec, Spec);
	MYSQL_SYNTHESIZE(string, unit, Unit);
	MYSQL_SYNTHESIZE(double, price, Price);
	MYSQL_SYNTHESIZE(int, quantity, Quantity);
	MYSQL_SYNTHESIZE(double, subtotal, Subtotal);
	MYSQL_SYNTHESIZE(string, chargeType, ChargeType);
	MYSQL_SYNTHESIZE(string, operatorId, Operator);
	MYSQL_SYNTHESIZE(string, chargeTime, ChargeTime);
	MYSQL_SYNTHESIZE(string, chargeDept, ChargeDept);
	MYSQL_SYNTHESIZE(string, chargeSource, ChargeSource);
	MYSQL_SYNTHESIZE(string, supplementReason, SupplementReason);
	MYSQL_SYNTHESIZE(string, status, Status);

public:
	InpatientChargeRecordDO() : BaseDO("inpatient_charge_record")
	{
		MYSQL_ADD_FIELD_PK("id", "ll", id);
		MYSQL_ADD_FIELD("charge_no", "s", chargeNo);
		MYSQL_ADD_FIELD("inp_no", "s", inpNo);
		MYSQL_ADD_FIELD("order_id", "s", orderId);
		MYSQL_ADD_FIELD("item_code", "s", itemCode);
		MYSQL_ADD_FIELD("item_name", "s", itemName);
		MYSQL_ADD_FIELD("spec", "s", spec);
		MYSQL_ADD_FIELD("unit", "s", unit);
		MYSQL_ADD_FIELD("price", "d", price);
		MYSQL_ADD_FIELD("quantity", "i", quantity);
		MYSQL_ADD_FIELD("subtotal", "d", subtotal);
		MYSQL_ADD_FIELD("charge_type", "s", chargeType);
		MYSQL_ADD_FIELD("operator", "s", operatorId);
		MYSQL_ADD_FIELD("charge_time", "s", chargeTime);
		MYSQL_ADD_FIELD("charge_dept", "s", chargeDept);
		MYSQL_ADD_FIELD("charge_source", "s", chargeSource);
		MYSQL_ADD_FIELD("supplement_reason", "s", supplementReason);
		MYSQL_ADD_FIELD("status", "s", status);
	}
};

typedef std::shared_ptr<InpatientChargeRecordDO> PtrInpatientChargeRecordDO;

#endif // !_INPATIENT_CHARGE_RECORD_DO_
