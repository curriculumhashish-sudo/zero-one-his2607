#pragma once
#ifndef _CHARGE_CONFIRM_CONTROLLER_H_
#define _CHARGE_CONFIRM_CONTROLLER_H_
#include "../../domain/dto/register/ChargeConfirmDTO.h"
#include "../../domain/vo/register/ChargeConfirmVO.h"
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("macchiato.charging.register.tag.tag1")

class ChargeConfirmController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(ChargeConfirmController);
public:

    // 定义展示收费详情
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("macchiato.charging.register.summary1"),
        confirmCharge,
        StringJsonVO::Wrapper,
        API_TAG
    );
    // 定义展示收费详情端点处理逻辑
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/charge/confirm",
        confirmCharge,
        BODY_DTO(ChargeConfirmDTO::Wrapper, dto),
        execConfirmCharge(dto, authObject->getPayload())
    );

private:
    // 定义执行确认收费接口
    ChargeConfirmJsonVO::Wrapper execConfirmCharge(const ChargeConfirmDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_CHARGE_CONFIRM_CONTROLLER_H_