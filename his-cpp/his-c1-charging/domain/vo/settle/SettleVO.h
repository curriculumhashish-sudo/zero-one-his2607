#pragma once

#ifndef _SETTLE_VO_H_
#define _SETTLE_VO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/settle/SettleDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettleJsonVO : public JsonVO<SettleResultDTO::Wrapper>
{
	DTO_INIT(SettleJsonVO, JsonVO<SettleResultDTO::Wrapper>);

public:
	SettleJsonVO()
	{
		this->data = SettleResultDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SETTLE_VO_H_
