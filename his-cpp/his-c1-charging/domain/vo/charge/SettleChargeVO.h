#pragma once

#ifndef _SETTLE_CHARGE_VO_
#define _SETTLE_CHARGE_VO_

#include "../../GlobalInclude.h"
#include "domain/dto/charge/PreSettleChargeDTO.h"
#include "domain/dto/charge/ConfirmSettleChargeDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   预结算响应视图对象
*/
class PreSettleChargeVO : public JsonVO<PreSettleChargeResponseDTO::Wrapper>
{
    DTO_INIT(PreSettleChargeVO, JsonVO<PreSettleChargeResponseDTO::Wrapper>);
};

/*
   正式结算响应视图对象
*/
class ConfirmSettleChargeVO : public JsonVO<ConfirmSettleChargeResponseDTO::Wrapper>
{
    DTO_INIT(ConfirmSettleChargeVO, JsonVO<ConfirmSettleChargeResponseDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SETTLE_CHARGE_VO_
