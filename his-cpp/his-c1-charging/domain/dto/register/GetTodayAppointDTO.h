#ifndef _GET_TODAY_APPOINT_DTO_
#define _GET_TODAY_APPOINT_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetTodayAppointDTO :public oatpp::DTO
{
	DTO_INIT(GetTodayAppointDTO, DTO);

	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.today_id");
	}

	DTO_FIELD(String, status);
	DTO_FIELD_INFO(status) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.status");
	}

	DTO_FIELD(UInt32, capacity);
	DTO_FIELD_INFO(capacity) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.capacity");
	}

	DTO_FIELD(UInt32, booked_count);
	DTO_FIELD_INFO(booked_count) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.bookled_count");
	}

	DTO_FIELD(String, register_type);
	DTO_FIELD_INFO(register_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.register_type");
	}

	DTO_FIELD(String, organization);
	DTO_FIELD_INFO(organization) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.organization");
	}

	DTO_FIELD(String, start_time);
	DTO_FIELD_INFO(start_time) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.start_time");
	}

	DTO_FIELD(String, end_time);
	DTO_FIELD_INFO(end_time) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.end_time");
	}
	DTO_FIELD(String, serviceTypeId);
	DTO_FIELD_INFO(serviceTypeId) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.serviceTypeId");
	}
	DTO_FIELD(Float64, cost);
	DTO_FIELD_INFO(cost) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.cost");
	}
	DTO_FIELD(String, practitionerId);
	DTO_FIELD_INFO(practitionerId) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.doctor_id");
	}
	DTO_FIELD(String, org_id);
	DTO_FIELD_INFO(org_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging,register.organization_id");
	}


	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

class GetTodayAppointPageDTO : public PageDTO<GetTodayAppointDTO::Wrapper>
{
	DTO_INIT(GetTodayAppointPageDTO, PageDTO<GetTodayAppointDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
