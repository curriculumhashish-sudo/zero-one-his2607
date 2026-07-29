#pragma once
#ifndef _CHARGE_CONFIRM_SERVICE_
#define _CHARGE_CONFIRM_SERVICE_

#include "../../domain/dto/register/ChargeConfirmDTO.h"
#include "domain/dto/PayloadDTO.h"

class ChargeConfirmService {
public:
    bool confirmPayment(const ChargeConfirmDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_CHARGE_CONFIRM_SERVICE_
