#pragma once

#ifndef _ADD_REGISTER_VO_
#define _ADD_REGISTER_VO_

#include "../../GlobalInclude.h"
#include "../../dto/register/AddRegisterDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class AddRegisterJsonVO : public JsonVO<ResultDTO::Wrapper> {
	DTO_INIT(AddRegisterJsonVO, JsonVO<ResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif