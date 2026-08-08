#ifndef __ADD_REGISTER_SERVICE__
#define __ADD_REGISTER_SERVICE__
#include "domain/dto/register/AddRegisterDTO.h"
#include "../../dao/register/AddRegisterDAO.h"
#include "domain/do/register/AddRegisterDO.h"
#include "id/UuidFacade.h"

class AddRegisterService
{
public:
	std::string Addregister(const AddRegisterDTO::Wrapper& dto)
	{
		EncounterDO encounter;
		ParticipantDO part;
		LocationDO loc;
		PatientDO pa;
		SlotDO slot;
		AccountDO account;
		UuidFacade uf;
		AddRegisterDAO dao;
		std::string now = GetCurrentDateTime();
		std::string encounter_id = uf.genUuid();
		std::string part_id = uf.genUuid();
		std::string loc_id = uf.genUuid();
		std::string account_id = uf.genUuid();
		std::string pa_id = dto->patient_id.getValue("");
		if(pa_id=="")
		{
			pa_id = uf.genUuid();
			pa.setId(pa_id);
			ZO_STAR_DOMAIN_DTO_TO_DO(pa, dto, Created_by, operator_id, Tenant_id, tenant_id, Name, name, Id_card, id_number, Phone, phone, Gender_enum, sex, Birth_date, birth_date, Organization_id, organization_id)
			pa.setCreated_at(now);
			pa.setUpdated_at(now);
			pa.setIs_deleted(0);
			pa.setVersion(0);
			pa.setBus_no("PA-" + GetCurrentDate() + "-" + uf.genUuid());
			pa.setStatus_enum("ACTIVE");
			pa.setActive_flag(1);
			pa.setTemp_flag(0);
			if (dao.insert(pa) != 1)return "";
			cout << "pa seccuss" << endl;
		}

		ZO_STAR_DOMAIN_DTO_TO_DO(encounter, dto, Service_type_id, serviceTypeId, Organization_id,organization_id, Created_by, operator_id,Registrar_id,operator_id, Tenant_id, tenant_id);
		encounter.setId(encounter_id);
		encounter.setPatient_id(pa_id);
		encounter.setCreated_at(now);
		encounter.setUpdated_at(now);
		encounter.setIs_deleted(0);
		encounter.setVersion(0);
		encounter.setBus_no("EN-" + GetCurrentDate() + "-" + uf.genUuid());
		encounter.setStatus_enum("IN_PROGRESS");
		encounter.setClass_enum("AMB");
		encounter.setStart_time(now);
		if (dao.insert(encounter) != 1)return "";
		cout << "encounter seccuss" << endl;

		ZO_STAR_DOMAIN_DTO_TO_DO(part, dto, Created_by, operator_id, Tenant_id,tenant_id, Practitioner_id,doctor_id, Role_code, role_code);
		part.setId(part_id);
		part.setEncounter_id(encounter_id);
		part.setCreated_at(now);
		part.setUpdated_at(now);
		part.setIs_deleted(0);
		part.setVersion(0);
		part.setStatus_enum("ACTIVE");
		part.setStart_time(now);
		if (dao.insert(part) != 1)return "";
		cout << "part seccuss" << endl;

		ZO_STAR_DOMAIN_DTO_TO_DO(loc, dto, Created_by, operator_id, Tenant_id, tenant_id, Location_id, organization_id);
		loc.setId(loc_id);
		loc.setEncounter_id(encounter_id);
		loc.setCreated_at(now);
		loc.setUpdated_at(now);
		loc.setIs_deleted(0);
		loc.setVersion(0);
		loc.setStatus_enum("ACTIVE");
		loc.setStart_time(now);
		loc.setForm_enum("HOUSE");
		if (dao.insert(loc) != 1)return "";
		cout << "loc seccuss" << endl;

		ZO_STAR_DOMAIN_DTO_TO_DO(slot,dto, Id, slotId);
		if (dao.update_slot(slot) <0)return "";
		cout << "slot seccuss" << endl;

		ZO_STAR_DOMAIN_DTO_TO_DO(account, dto, Created_by, operator_id, Tenant_id, tenant_id, Contract_no, contractNo, Balance,balance,Type_code,type_code);
		account.setId(account_id);
		account.setEncounter_id(encounter_id);
		account.setCreated_at(now);
		account.setUpdated_at(now);
		account.setIs_deleted(0);
		account.setVersion(0);
		account.setStatus_enum("ACTIVE");
		account.setPatient_id(pa_id);
		account.setBus_no("AC" + GetCurrentDate() + uf.genUuid());
		account.setBilling_status_enum("open");
		account.setDefault_flag(1);
		if (dao.insert(account) != 1)return "";
		cout << "account seccuss" << endl;
		return encounter_id;
	}
private:
	std::string GetCurrentDate() {
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%Y-%m-%d");
		return oss.str();
	}

	std::string GetCurrentDateTime() {
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
		return oss.str();
	}
};

#endif