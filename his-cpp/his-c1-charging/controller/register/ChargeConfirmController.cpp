#include "stdafx.h"
#include "ChargeConfirmController.h"
#include "../../service/register/ChargeConfirmService.h"

ChargeConfirmJsonVO::Wrapper ChargeConfirmController::execConfirmCharge(const ChargeConfirmDTO::Wrapper& dto, const PayloadDTO& payload)
{
    ChargeConfirmService service;
    bool ok = service.confirmPayment(dto, payload);
    auto vo = ChargeConfirmJsonVO::createShared();
    if (ok)
    {
        vo->success(dto);
    }
    else
    {
        vo->fail(dto);
    }
    return vo;
}
