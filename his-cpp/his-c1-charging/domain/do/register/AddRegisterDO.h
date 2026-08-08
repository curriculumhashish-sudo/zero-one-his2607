#ifndef _ADD_REEGISTER_DO_
#define _ADD_REEGISTER_DO_
#include "../DoInclude.h"


class EncounterDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(std::string, created_at, Created_at);
	MYSQL_SYNTHESIZE(std::string, updated_at, Updated_at);
	MYSQL_SYNTHESIZE(std::string, created_by, Created_by);
	MYSQL_SYNTHESIZE(int, is_deleted, Is_deleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, bus_no, Bus_no);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(std::string, patient_id, Patient_id);
	MYSQL_SYNTHESIZE(std::string, class_enum, Class_enum);
	MYSQL_SYNTHESIZE(std::string, service_type_id, Service_type_id);
	MYSQL_SYNTHESIZE(std::string, start_time, Start_time);
	MYSQL_SYNTHESIZE(std::string, organization_id, Organization_id);
	MYSQL_SYNTHESIZE(std::string, registrar_id, Registrar_id);
public:
	EncounterDO() : BaseDO("adm_encounter") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD("updated_at", "dt", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("is_deleted", "i", is_deleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("class_enum", "s", class_enum);
		MYSQL_ADD_FIELD("service_type_id", "s", service_type_id);
		MYSQL_ADD_FIELD("start_time", "dt", start_time);
		MYSQL_ADD_FIELD("organization_id", "s", organization_id);
		MYSQL_ADD_FIELD("registrar_id", "s", registrar_id);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
	}
};

class ParticipantDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(std::string, created_at, Created_at);
	MYSQL_SYNTHESIZE(std::string, updated_at, Updated_at);
	MYSQL_SYNTHESIZE(std::string, created_by, Created_by);
	MYSQL_SYNTHESIZE(int, is_deleted, Is_deleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, encounter_id, Encounter_id);
	MYSQL_SYNTHESIZE(std::string, role_code, Role_code);
	MYSQL_SYNTHESIZE(std::string, practitioner_id, Practitioner_id);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(std::string, start_time, Start_time);
public:
	ParticipantDO() : BaseDO("adm_encounter_participant") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD("updated_at", "dt", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("is_deleted", "i", is_deleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("role_code", "s", role_code);
		MYSQL_ADD_FIELD("practitioner_id", "s", practitioner_id);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("start_time", "dt", start_time);
	}
};

class LocationDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(std::string, created_at, Created_at);
	MYSQL_SYNTHESIZE(std::string, updated_at, Updated_at);
	MYSQL_SYNTHESIZE(std::string, created_by, Created_by);
	MYSQL_SYNTHESIZE(int, is_deleted, Is_deleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, encounter_id, Encounter_id);
	MYSQL_SYNTHESIZE(std::string, location_id, Location_id);
	MYSQL_SYNTHESIZE(std::string, form_enum, Form_enum);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(std::string, start_time, Start_time);
public:
	LocationDO() : BaseDO("adm_encounter_location") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD("updated_at", "dt", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("is_deleted", "i", is_deleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("location_id", "s", location_id);
		MYSQL_ADD_FIELD("form_enum", "s", form_enum);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("start_time", "dt", start_time);
	}
};

class PatientDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(std::string, created_at, Created_at);
	MYSQL_SYNTHESIZE(std::string, updated_at, Updated_at);
	MYSQL_SYNTHESIZE(std::string, created_by, Created_by);
	MYSQL_SYNTHESIZE(int, is_deleted, Is_deleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, bus_no, Bus_no);
	MYSQL_SYNTHESIZE(std::string, name, Name);
	MYSQL_SYNTHESIZE(std::string, gender_enum, Gender_enum);
	MYSQL_SYNTHESIZE(std::string, birth_date, Birth_date);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(int, active_flag, Active_flag);
	MYSQL_SYNTHESIZE(int, temp_flag, Temp_flag);
	MYSQL_SYNTHESIZE(std::string, id_card, Id_card);
	MYSQL_SYNTHESIZE(std::string, phone, Phone);
	MYSQL_SYNTHESIZE(std::string, organization_id, Organization_id);
public:
	PatientDO() : BaseDO("adm_patient") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD("updated_at", "dt", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("is_deleted", "i", is_deleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("gender_enum", "s", gender_enum);
		MYSQL_ADD_FIELD("birth_date", "dt", birth_date);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("active_flag", "i", active_flag);
		MYSQL_ADD_FIELD("temp_flag", "i", temp_flag);
		MYSQL_ADD_FIELD("id_card", "s", id_card);
		MYSQL_ADD_FIELD("phone", "s", phone);
		MYSQL_ADD_FIELD("organization_id", "s", organization_id);
	}
};

class SlotDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);

public:
	SlotDO() : BaseDO("wkf_slot") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
	}
};

class AccountDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(std::string, created_at, Created_at);
	MYSQL_SYNTHESIZE(std::string, updated_at, Updated_at);
	MYSQL_SYNTHESIZE(std::string, created_by, Created_by);
	MYSQL_SYNTHESIZE(int, is_deleted, Is_deleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, patient_id, Patient_id);
	MYSQL_SYNTHESIZE(std::string, encounter_id, Encounter_id);
	MYSQL_SYNTHESIZE(std::string, type_code, Type_code);
	MYSQL_SYNTHESIZE(std::string, bus_no, Bus_no);
	MYSQL_SYNTHESIZE(double, balance, Balance);
	MYSQL_SYNTHESIZE(std::string, billing_status_enum, Billing_status_enum);
	MYSQL_SYNTHESIZE(std::string, contract_no, Contract_no);
	MYSQL_SYNTHESIZE(int, default_flag, Default_flag);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
public:
	AccountDO() : BaseDO("adm_account") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD("updated_at", "dt", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("is_deleted", "i", is_deleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("type_code", "s", type_code);
		MYSQL_ADD_FIELD("balance", "d", balance);
		MYSQL_ADD_FIELD("billing_status_enum", "s", billing_status_enum);
		MYSQL_ADD_FIELD("contract_no", "s", contract_no);
		MYSQL_ADD_FIELD("default_flag", "i", default_flag);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
	}
};
#endif // !_SAMPLE_DO_