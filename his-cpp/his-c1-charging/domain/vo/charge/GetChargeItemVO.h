#pragma once

#ifndef _GET_CHARGE_ITEM_VO_
#define _GET_CHARGE_ITEM_VO_

#include "../../GlobalInclude.h"
#include "domain/dto/charge/GetChargeItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   获取患者缴费项响应视图对象
*/
class GetChargeItemVO : public JsonVO<GetChargeItemDTO::Wrapper>
{
    DTO_INIT(GetChargeItemVO, JsonVO<GetChargeItemDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_CHARGE_ITEM_VO_
