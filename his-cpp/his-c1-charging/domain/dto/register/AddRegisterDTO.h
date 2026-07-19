#ifndef _ADD_REGISTER_DTO_
#define _ADD_REGISTER_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class AddRegisterDTO :public oatpp::DTO
{
	DTO_INIT(AddRegisterDTO, DTO);

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.name");
	}

	DTO_FIELD(UInt32, id_number);
	DTO_FIELD_INFO(id_number) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.id_number");
	}

	DTO_FIELD(UInt32, phone);
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

	DTO_FIELD(String, oganization);
	DTO_FIELD_INFO(oganization) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.oganization");
	}

	DTO_FIELD(String, docker);
	DTO_FIELD_INFO(docker) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.docker");
	}

	DTO_FIELD(String, cost_type);
	DTO_FIELD_INFO(cost_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.cost_type");
	}

	DTO_FIELD(String, register_type);
	DTO_FIELD_INFO(register_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.register_type");
	}


	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

#include OATPP_CODEGEN_END(DTO)
#endif