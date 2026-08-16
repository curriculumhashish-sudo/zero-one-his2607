#pragma once

#ifndef _REFUNDVO_H_
#define _REFUNDVO_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/doctor/Refund2DetailDTO.h"
#include "domain/dto/doctor/Refund2ApplyResultDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2DetailJsonVO : public ListJsonVO<Refund2DetailDTO::Wrapper>
{
    DTO_INIT(Refund2DetailJsonVO, ListJsonVO<Refund2DetailDTO::Wrapper>);
};

class Refund2ApplyResultJsonVO : public JsonVO<Refund2ApplyResultDTO::Wrapper>
{
    DTO_INIT(Refund2ApplyResultJsonVO, JsonVO<Refund2ApplyResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
