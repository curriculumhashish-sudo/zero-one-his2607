#pragma once
#ifndef _INOUTBOUNDVO_H_
#define _INOUTBOUNDVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/data_statistics/InOutboundDto.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// œ‘ æJsonVO
class InOutboundVo : public JsonVO<InOutboundDto::Wrapper> {
	DTO_INIT(InOutboundVo, JsonVO<InOutboundDto::Wrapper>);
};

// ∑÷“≥œ‘ æJsonVO
class InOutboundPageJsonVo : public JsonVO<InOutboundPageDto::Wrapper> {
	DTO_INIT(InOutboundPageJsonVo, JsonVO<InOutboundPageDto::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INOUTBOUNDVO_H_
