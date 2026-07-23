#pragma once

#ifndef _GET_HISTORY_LIST_VO_
#define _GET_HISTORY_LIST_VO_

#include "../../GlobalInclude.h"
#include "../../dto/settle/GetHistorySettleListDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetHistorySettleListVO : public JsonVO<GetHistorySettleListDTO::Wrapper> {
	DTO_INIT(GetHistorySettleListVO, JsonVO<GetHistorySettleListDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif