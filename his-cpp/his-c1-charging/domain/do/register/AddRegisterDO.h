#ifndef _ADD_REEGISTER_DO_
#define _ADD_REEGISTER_DO_
#include "../DoInclude.h"

/**
 * 示例数据库实体类
 */
class AddRegisterDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, name, Name);
	MYSQL_SYNTHESIZE(std::string, id_number, Id_number);
	MYSQL_SYNTHESIZE(std::string, phone, Phone);
	MYSQL_SYNTHESIZE(std::string, sex, Sex);
	MYSQL_SYNTHESIZE(std::string, birth_date, Birth_date);
	MYSQL_SYNTHESIZE(unsigned int, age, Age);
	MYSQL_SYNTHESIZE(std::string, organization, Organization);
	MYSQL_SYNTHESIZE(std::string, doctor, Doctor);
	MYSQL_SYNTHESIZE(std::string, patient_id, Patient_id);
	MYSQL_SYNTHESIZE(std::string, doctor_id, Doctor_id);
	MYSQL_SYNTHESIZE(std::string, organization_id, Organization_id);
	MYSQL_SYNTHESIZE(std::string, operator_id, Operator_id);
	MYSQL_SYNTHESIZE(std::string, tenant_id, Tenant_id);
	MYSQL_SYNTHESIZE(std::string, encounter_bus_no, Encounter_bus_no);
	MYSQL_SYNTHESIZE(std::string, patient_bus_no, Patient_bus_no);
	MYSQL_SYNTHESIZE(std::string, account_bus_no, Account_bus_no);
	MYSQL_SYNTHESIZE(std::string, now, Now);
	MYSQL_SYNTHESIZE(std::string, participant_id, Participant_id);
	MYSQL_SYNTHESIZE(std::string, location_id, Location_id); 
	MYSQL_SYNTHESIZE(std::string, account_id, Account_id);
	MYSQL_SYNTHESIZE(std::string, serviceTypeId, ServiceTypeId);
	MYSQL_SYNTHESIZE(std::string, slotId, SlotId);
	MYSQL_SYNTHESIZE(std::string, contractNo, ContractNo);
public:
	AddRegisterDO() : BaseDO("AddRegister") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("id_number", "s", id_number);
		MYSQL_ADD_FIELD("phone", "s", phone);
		MYSQL_ADD_FIELD("sex", "s", sex);
		MYSQL_ADD_FIELD("birth_date", "dt", birth_date);
		MYSQL_ADD_FIELD("age", "ui", age);
		MYSQL_ADD_FIELD("organization", "s", organization);
		MYSQL_ADD_FIELD("doctor", "s", doctor);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("doctor_id", "s", doctor_id);
		MYSQL_ADD_FIELD("organization_id", "s", organization_id);
		MYSQL_ADD_FIELD("operator_id", "s", operator_id);
		MYSQL_ADD_FIELD("encounter_bus_no", "s", encounter_bus_no);
		MYSQL_ADD_FIELD(" patient_bus_no", "s", patient_bus_no);
		MYSQL_ADD_FIELD("now", "dt", now);
		MYSQL_ADD_FIELD("participant_id", "s", participant_id);
		MYSQL_ADD_FIELD("location_id", "s", location_id);
		MYSQL_ADD_FIELD("serviceTypeId", "s", serviceTypeId);
		MYSQL_ADD_FIELD("slotId", "s", slotId);
		MYSQL_ADD_FIELD("contractNo", "s", contractNo);
		MYSQL_ADD_FIELD("account_id", "s", account_id);
		MYSQL_ADD_FIELD("account_bus_no", "s", account_bus_no);
	}
};

#endif // !_SAMPLE_DO_