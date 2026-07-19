#ifndef _CHARGE_CONFIRM_VO_
#define _CHARGE_CONFIRM_VO_
#include "../../GlobalInclude.h"
#include "../../dto/register/ChargeConfirmDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 确认收费响应VO
 */
class ChargeConfirmJsonVO : public JsonVO<ChargeConfirmDTO::Wrapper> {
    DTO_INIT(ChargeConfirmJsonVO, JsonVO<ChargeConfirmDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHARGE_CONFIRM_VO_