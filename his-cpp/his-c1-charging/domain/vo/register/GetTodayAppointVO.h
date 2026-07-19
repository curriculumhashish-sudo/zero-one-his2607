#pragma once

#ifndef _GET_TODAY_APPOINT_VO_
#define _GET_TODAY_APPOINT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/register/GetTodayAppointDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetTodayAppointJsonVO : public JsonVO<GetTodayAppointPageDTO::Wrapper> {
	DTO_INIT(GetTodayAppointJsonVO, JsonVO<GetTodayAppointPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif