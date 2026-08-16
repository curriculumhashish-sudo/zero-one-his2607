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

class ChargItemDO : public BaseDO
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
	MYSQL_SYNTHESIZE(std::string, bus_no, Bus_no);
	MYSQL_SYNTHESIZE(std::string, account_id, Account_id);
	MYSQL_SYNTHESIZE(std::string, generate_source_enum, Generate_source_enum);
	MYSQL_SYNTHESIZE(int, context_enum, Context_enum);
	MYSQL_SYNTHESIZE(std::string, service_table, Service_table);
	MYSQL_SYNTHESIZE(std::string, service_id, Service_id);
	MYSQL_SYNTHESIZE(std::string, definition_id, Definition_id);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(double, total_price, Total_price);
	MYSQL_SYNTHESIZE(std::string, occurrence_time, Occurrence_time);
	MYSQL_SYNTHESIZE(std::string, requesting_org_id, Requesting_org_id);
	MYSQL_SYNTHESIZE(double, quantity_value, Quantity_value);
	MYSQL_SYNTHESIZE(std::string, quantity_unit, Quantity_unit);
	MYSQL_SYNTHESIZE(double, unit_price, Unit_price);
	MYSQL_SYNTHESIZE(double, base_amount, Base_amount);
	MYSQL_SYNTHESIZE(std::string, enterer_id, Enterer_id);
	MYSQL_SYNTHESIZE(std::string, entered_date, Entered_date);
	MYSQL_SYNTHESIZE(std::string, performer_id, Performer_id);
public:
	ChargItemDO() : BaseDO("adm_charge_item") {
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
		MYSQL_ADD_FIELD("account_id", "s", account_id);
		MYSQL_ADD_FIELD("generate_source_enum", "s", generate_source_enum);
		MYSQL_ADD_FIELD("context_enum", "i", context_enum);
		MYSQL_ADD_FIELD("service_table", "s", service_table);
		MYSQL_ADD_FIELD("service_id", "s", service_id);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("total_price", "d", total_price);
		MYSQL_ADD_FIELD("occurrence_time", "dt", occurrence_time);
		MYSQL_ADD_FIELD("requesting_org_id", "s", requesting_org_id);
		MYSQL_ADD_FIELD("quantity_value", "d", quantity_value);
		MYSQL_ADD_FIELD("quantity_unit", "s", quantity_unit);
		MYSQL_ADD_FIELD("unit_price", "d", unit_price);
		MYSQL_ADD_FIELD("base_amount", "d", base_amount);
		MYSQL_ADD_FIELD("enterer_id", "s", enterer_id);
		MYSQL_ADD_FIELD("entered_date", "dt", entered_date);
		MYSQL_ADD_FIELD("performer_id", "s", performer_id);


		this->setGenerate_source_enum("registration");
		this->setContext_enum(4);
		this->setService_table("adm_healthcare_service");
		this->setStatus_enum("DRAFT");
		this->setQuantity_unit("1");
		this->setQuantity_value(1);
	}
};

class PaymentDO : public BaseDO
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
	MYSQL_SYNTHESIZE(std::string, payment_no, Payment_no);
	MYSQL_SYNTHESIZE(std::string, account_id, Account_id);
	MYSQL_SYNTHESIZE(std::string, status_enum, Status_enum);
	MYSQL_SYNTHESIZE(std::string, enterer_id, Enterer_id);
	MYSQL_SYNTHESIZE(std::string, payment_enum, Payment_enum);
	MYSQL_SYNTHESIZE(std::string, kind_enum, Kind_enum);
	MYSQL_SYNTHESIZE(double, tendered_amount, Tendered_amount);
	MYSQL_SYNTHESIZE(int, print_count, Print_count);
	MYSQL_SYNTHESIZE(std::string, charge_item_ids, Charge_item_ids);
public:
	PaymentDO() : BaseDO("fin_payment_reconciliation") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD("updated_at", "dt", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		MYSQL_ADD_FIELD("is_deleted", "i", is_deleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("payment_no", "s", payment_no);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("account_id", "s", account_id);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("enterer_id", "s", enterer_id);
		MYSQL_ADD_FIELD("payment_enum", "s", payment_enum);
		MYSQL_ADD_FIELD("kind_enum", "s", kind_enum);
		MYSQL_ADD_FIELD("tendered_amount", "d", tendered_amount);
		MYSQL_ADD_FIELD("print_count", "i", print_count);
		MYSQL_ADD_FIELD("charge_item_ids", "s", charge_item_ids);

		this->setStatus_enum("DRAFT");
		this->setPayment_enum("UN_PAY");
		this -> setKind_enum("OUTPATIENT_CLINIC");
		this->setPrint_count(0);
	}
};

class DefinitionDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(double, price_value, Price_value);
public:
	DefinitionDO() : BaseDO("adm_charge_item_definition") {
		MYSQL_ADD_FIELD("id", "s", id);
		MYSQL_ADD_FIELD("price_value", "d", price_value);
	}
};
#endif // !_SAMPLE_DO_