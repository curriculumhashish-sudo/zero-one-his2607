#ifndef __ADD_REGISTER_SERVICE__
#define __ADD_REGISTER_SERVICE__
#include "domain/dto/register/AddRegisterDTO.h"
#include"../../dao/register/AddRegisterDAO.h"
#include "domain/do/register/AddRegisterDO.h"
#include "id/UuidFacade.h"

class AddRegisterService
{
public:
	std::string Addregister(const AddRegisterDTO::Wrapper& dto)
	{
		AddRegisterDO data;
		ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Name, name, Id_number, id_number, Phone, phone, Sex, sex, Birth_date, birth_date, Age, age, Organization, organization, Doctor, doctor);
		ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Patient_id, patient_id, Doctor_id, doctor_id, Organization_id, organization_id);
		ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Operator_id, operator_id, Tenant_id, tenant_id);
		UuidFacade uf;
		data.setId(uf.genUuid());
		data.setParticipant_id(uf.genUuid());
		data.setLocation_id(uf.genUuid());
		data.setEncounter_bus_no("EN-" + GetCurrentDate() + "-"+uf.genUuid());
		data.setPatient_bus_no("PA"+ uf.genUuid());
		data.setNow(GetCurrentDateTime());
		AddRegisterDAO dao;
		uint64_t result=dao.InsertRegister(data);
		return result ==1 ? data.getId() : "";
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