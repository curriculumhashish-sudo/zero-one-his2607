#pragma once

#ifndef _GET_COST_DETAIL_VO_H_
#define _GET_COST_DETAIL_VO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/settle/GetCostDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetCostDetailJsonVO : public JsonVO<GetCostDetailDTO::Wrapper>
{
	DTO_INIT(GetCostDetailJsonVO, JsonVO<GetCostDetailDTO::Wrapper>);

public:
	GetCostDetailJsonVO()
	{
		this->data = GetCostDetailDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_COST_DETAIL_VO_H_
