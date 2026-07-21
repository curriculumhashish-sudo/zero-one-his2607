#pragma once

#ifndef _GET_SEE_VO_
#define _GET_SEE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/register/GetSeeDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSeePageJsonVO : public JsonVO<GetSeePageDTO::Wrapper> {
	DTO_INIT(GetSeePageJsonVO, JsonVO<GetSeePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif