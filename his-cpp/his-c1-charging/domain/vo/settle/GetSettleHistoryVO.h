#pragma once

#ifndef _GET_SETTLE_HISTORY_VO_H_
#define _GET_SETTLE_HISTORY_VO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/settle/GetSettleHistoryDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSettleHistoryJsonVO : public JsonVO<GetSettleHistoryPageDTO::Wrapper>
{
	DTO_INIT(GetSettleHistoryJsonVO, JsonVO<GetSettleHistoryPageDTO::Wrapper>);

public:
	GetSettleHistoryJsonVO()
	{
		this->data = GetSettleHistoryPageDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_SETTLE_HISTORY_VO_H_
