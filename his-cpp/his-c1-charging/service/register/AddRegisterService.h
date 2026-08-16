#ifndef __ADD_REGISTER_SERVICE__
#define __ADD_REGISTER_SERVICE__
#include "domain/dto/register/AddRegisterDTO.h"
#include "../../dao/register/AddRegisterDAO.h"
#include "domain/do/register/AddRegisterDO.h"
#include "id/UuidFacade.h"

class AddRegisterService
{
public:
	ResultDTO::Wrapper Addregister(const AddRegisterDTO::Wrapper& dto)
	{
		EncounterDO encounter;
		ParticipantDO part;
		LocationDO loc;
		PatientDO pa;
		SlotDO slot;
		AccountDO account;
		ChargItemDO chageItem;
		PaymentDO pay;
		UuidFacade uf;
		AddRegisterDAO dao;
		DefinitionDO def = dao.getCost(dto->serviceTypeId.getValue(""));
		if(def.getIdPtr() == nullptr) return nullptr;
		std::string now = GetCurrentDateTime();
		std::string encounter_id = uf.genUuid();
		std::string part_id = uf.genUuid();
		std::string loc_id = uf.genUuid();
		std::string account_id = uf.genUuid();
		std::string charge_id = uf.genUuid();
		std::string pay_id = uf.genUuid();
		std::string pa_id = dto->patient_id.getValue("");
		bool is_insert_pa = false;
		if(pa_id=="")
		{
			is_insert_pa = true;
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

		ZO_STAR_DOMAIN_DTO_TO_DO(part, dto, Created_by, operator_id, Tenant_id,tenant_id, Practitioner_id,doctor_id, Role_code, role_code);
		part.setId(part_id);
		part.setEncounter_id(encounter_id);
		part.setCreated_at(now);
		part.setUpdated_at(now);
		part.setIs_deleted(0);
		part.setVersion(0);
		part.setStatus_enum("ACTIVE");
		part.setStart_time(now);

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

		ZO_STAR_DOMAIN_DTO_TO_DO(slot,dto, Id, slotId);

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
		
		ZO_STAR_DOMAIN_DTO_TO_DO(chageItem, dto, Service_id, serviceTypeId, Requesting_org_id, organization_id, Created_by, operator_id, Enterer_id, operator_id, Tenant_id, tenant_id);
		ZO_STAR_DOMAIN_DTO_TO_DO(chageItem, dto, Performer_id,doctor_id);
		chageItem.setId(charge_id);
		chageItem.setEncounter_id(encounter_id);
		chageItem.setCreated_at(now);
		chageItem.setUpdated_at(now);
		chageItem.setIs_deleted(0);
		chageItem.setVersion(0);
		chageItem.setBus_no("CI-" + GetCurrentDate() + "-" + uf.genUuid());
		chageItem.setPatient_id(pa_id);
		chageItem.setAccount_id(account_id);
		chageItem.setOccurrence_time(now);
		chageItem.setEntered_date(now);
		chageItem.setDefinition_id(def.getId());
		chageItem.setTotal_price(def.getPrice_value());
		chageItem.setUnit_price(def.getPrice_value());
		chageItem.setBase_amount(def.getPrice_value());


		ZO_STAR_DOMAIN_DTO_TO_DO(pay, dto, Created_by, operator_id, Enterer_id, operator_id, Tenant_id, tenant_id);
		pay.setId(pay_id);
		pay.setEncounter_id(encounter_id);
		pay.setCreated_at(now);
		pay.setUpdated_at(now);
		pay.setIs_deleted(0);
		pay.setVersion(0);
		pay.setPayment_no("PMR-" + GetCurrentDate() + "-" + uf.genUuid());
		pay.setPatient_id(pa_id);
		pay.setAccount_id(account_id);
		pay.setTendered_amount(def.getPrice_value());
		pay.setCharge_item_ids(charge_id);

		if (dao.add_register(chageItem,pay,encounter, part, loc, pa, slot, account,is_insert_pa) != 1)return nullptr;
		auto result = ResultDTO::createShared();
		result->cost = def.getPrice_value();
		result->encounter_id = encounter_id;
		result->pay_id = pay_id;
		result->chargItem_id = charge_id;
		return result;
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