#pragma once
#ifndef _ACCOUNT_DO_
#define _ACCOUNT_DO_

#include "../DoInclude.h"

class AccountDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(double, balance, Balance);
	MYSQL_SYNTHESIZE(string, billingStatusEnum, BillingStatusEnum);
	MYSQL_SYNTHESIZE(string, contractNo, ContractNo);
	MYSQL_SYNTHESIZE(string, ybAreaNo, YbAreaNo);
	MYSQL_SYNTHESIZE(bool, encounterFlag, EncounterFlag);
	MYSQL_SYNTHESIZE(string, insutype, Insutype);
	MYSQL_SYNTHESIZE(bool, defaultFlag, DefaultFlag);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

public:
	AccountDO() : BaseDO("adm_account")
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
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, true);
		MYSQL_ADD_FIELD_NULLABLE("balance", "d", balance, true);
		MYSQL_ADD_FIELD_NULLABLE("billing_status_enum", "s", billingStatusEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("contract_no", "s", contractNo, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_area_no", "s", ybAreaNo, true);
		MYSQL_ADD_FIELD_NULLABLE("encounter_flag", "b", encounterFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("insutype", "s", insutype, true);
		MYSQL_ADD_FIELD_NULLABLE("default_flag", "b", defaultFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, true);
	}
};

typedef std::shared_ptr<AccountDO> PtrAccountDO;

#endif
