#pragma once

#ifndef _GET_YXY_LIST_VO_
#define _GET_YXY_LIST_VO_

#include "../../GlobalInclude.h"
#include "../../dto/settle/GetExpenseDetailsListDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetExpenseDetailsListVO : public JsonVO<GetExpenseDetailsListDTO::Wrapper> {
	DTO_INIT(GetExpenseDetailsListVO, JsonVO<GetExpenseDetailsListDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif