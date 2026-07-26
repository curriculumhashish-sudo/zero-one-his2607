#pragma once
#ifndef _AUTOGENRULEDO_
#define _AUTOGENRULEDO_

#include "../DoInclude.h"

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

#endif
