#ifndef _ADD_REGISTER_DTO_
#define _ADD_REGISTER_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class ResultDTO :public oatpp::DTO
{
	DTO_INIT(ResultDTO, DTO);

	DTO_FIELD(String, encounter_id);
	DTO_FIELD_INFO(encounter_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.encounter_id");
	}

	DTO_FIELD(String, chargItem_id);
	DTO_FIELD_INFO(chargItem_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.chargItem_id");
	}

	DTO_FIELD(String, pay_id);
	DTO_FIELD_INFO(pay_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.pay_id");
	}

	DTO_FIELD(Float64, cost);
	DTO_FIELD_INFO(cost) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.cost");
	}
};

class AddRegisterDTO :public oatpp::DTO
{
	DTO_INIT(AddRegisterDTO, DTO);

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.name");
	}

	DTO_FIELD(String, id_number);
	DTO_FIELD_INFO(id_number) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.id_number");
	}

	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.phone");
	}

	DTO_FIELD(String, sex);
	DTO_FIELD_INFO(sex) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.sex");
	}

	DTO_FIELD(String, birth_date);
	DTO_FIELD_INFO(birth_date) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.birth_date");
	}

	DTO_FIELD(UInt32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.age");
	}

	DTO_FIELD(String, organization);
	DTO_FIELD_INFO(organization) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.organization");
	}

	DTO_FIELD(String, doctor);
	DTO_FIELD_INFO(doctor) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.docker");
	}
	DTO_FIELD(String, patient_id);
	DTO_FIELD_INFO(patient_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.patient_id");
	}
	DTO_FIELD(String, doctor_id);
	DTO_FIELD_INFO(doctor_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.doctor_id");
	}
	DTO_FIELD(String, organization_id);
	DTO_FIELD_INFO(organization_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.organization_id");
	}
	DTO_FIELD(String, operator_id);
	DTO_FIELD_INFO(operator_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.operator_id");
	}
	DTO_FIELD(String, tenant_id);
	DTO_FIELD_INFO(tenant_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.tenant_id");
	}
	DTO_FIELD(String, serviceTypeId);
	DTO_FIELD_INFO(serviceTypeId) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.serviceTypeId");
	}
	DTO_FIELD(String, slotId);
	DTO_FIELD_INFO(slotId) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.slotId");
	}
	DTO_FIELD(String, contractNo);
	DTO_FIELD_INFO(contractNo) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.contractNo");
	}

	DTO_FIELD(String, role_code);
	DTO_FIELD_INFO(role_code) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.role_code");
	}
	DTO_FIELD(Float64, balance);
	DTO_FIELD_INFO(balance) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.balance");
	}
	DTO_FIELD(String, type_code);
	DTO_FIELD_INFO(type_code) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.type_code");
	}
	DTO_FIELD(Float64, discount);
	DTO_FIELD_INFO(discount) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.discount");
	}

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

#include OATPP_CODEGEN_END(DTO)
#endif