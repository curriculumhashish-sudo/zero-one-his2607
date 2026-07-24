#ifndef _GET_SEE_DTO_
#define _GET_SEE_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSeeDTO:public oatpp::DTO
{
	DTO_INIT(GetSeeDTO, DTO);

	DTO_FIELD(String, number);
	DTO_FIELD_INFO(number) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.number");
	}

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.name");
	}

	DTO_FIELD(String, sex);
	DTO_FIELD_INFO(sex) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.sex");
	}

	DTO_FIELD(UInt32, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.phone");
	}

	DTO_FIELD(String, oganization);
	DTO_FIELD_INFO(oganization) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.oganization");
	}

	DTO_FIELD(String, doctor);
	DTO_FIELD_INFO(doctor) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.doctor");
	}

	DTO_FIELD(String, cost_type);
	DTO_FIELD_INFO(cost_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.cost_type");
	}

	DTO_FIELD(String, addr);
	DTO_FIELD_INFO(addr) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.addr");
	}

	DTO_FIELD(String, register_type);
	DTO_FIELD_INFO(register_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.register_type");
	}

	DTO_FIELD(Float64, cost);
	DTO_FIELD_INFO(cost) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.cost");
	}

	DTO_FIELD(UInt64, time);
	DTO_FIELD_INFO(time) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.time");
	}
		
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

class GetSeePageDTO: public PageDTO<GetSeeDTO::Wrapper>
{
	DTO_INIT(GetSeePageDTO, PageDTO<GetSeeDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
