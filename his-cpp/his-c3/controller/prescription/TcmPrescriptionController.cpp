/*
 Copyright (c) 2025-2066, Zero One Technology Co., Ltd.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "TcmPrescriptionController.h"
#include "../../service/prescription/TcmPrescriptionService.h"

/**
 * Validates and delegates a prescription - signing request.
 */
TcmPrescriptionSignJsonVO::Wrapper
TcmPrescriptionController::execSignTcmPrescription(
    const TcmPrescriptionSignDTO::Wrapper & dto,
    const PayloadDTO & payload
)
{
    auto response = TcmPrescriptionSignJsonVO::createShared();

    if (!dto)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    const std::string validationError = dto->validate();

    if (!validationError.empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    TcmPrescriptionService service;
    const auto serviceResult =
        service.signPrescription(dto, payload);

    if (serviceResult.success)
    {
        response->success(serviceResult.data);
    }
    else if (serviceResult.invalidRequest)
    {
        response->init(
            serviceResult.data,
            RS_PARAMS_INVALID
        );
    }
    else
    {
        response->fail(serviceResult.data);
    }

    return response;
}


/**
 * Validates and delegates a prescription-withdrawal request.
 */
TcmPrescriptionWithdrawJsonVO::Wrapper TcmPrescriptionController::execWithdrawTcmPrescription(
    const TcmPrescriptionWithdrawDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto response = TcmPrescriptionWithdrawJsonVO::createShared();

    if (!dto)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    const std::string validationError = dto->validate();

    if (!validationError.empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    // Delegate the structurally valid request to the business service.
    TcmPrescriptionService service;
    const auto serviceResult = service.withdrawPrescription(dto, payload);

    if (serviceResult.success)
    {
        response->success(serviceResult.data);
    }
    else if (serviceResult.invalidRequest)
    {
        response->init(serviceResult.data, RS_PARAMS_INVALID);
    }
    else
    {
        response->fail(serviceResult.data);
    }

    return response;
}
/**
 * Validates and delegates a prescription-stop request.
 */
TcmPrescriptionStopJsonVO::Wrapper TcmPrescriptionController::execStopTcmPrescription(
    const TcmPrescriptionStopDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto response = TcmPrescriptionStopJsonVO::createShared();

    if (!dto)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    const std::string validationError = dto->validate();

    if (!validationError.empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    TcmPrescriptionService service;
    const auto serviceResult = service.stopPrescription(dto, payload);

    if (serviceResult.success)
    {
        response->success(serviceResult.data);
    }
    else if (serviceResult.invalidRequest)
    {
        response->init(serviceResult.data, RS_PARAMS_INVALID);
    }
    else
    {
        response->fail(serviceResult.data);
    }

    return response;
}

/**
 * Validates and delegates a cancel-stop request.
 */
TcmPrescriptionCancelStopJsonVO::Wrapper TcmPrescriptionController::execCancelStopTcmPrescription(
    const TcmPrescriptionCancelStopDTO::Wrapper &dto, const PayloadDTO &payload)
{
    auto response = TcmPrescriptionCancelStopJsonVO::createShared();

    if (!dto)
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    const std::string validationError = dto->validate();

    if (!validationError.empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    TcmPrescriptionService service;
    const auto serviceResult = service.cancelStopPrescription(dto, payload);

    if (serviceResult.success)
    {
        response->success(serviceResult.data);
    }
    else if (serviceResult.invalidRequest)
    {
        response->init(serviceResult.data, RS_PARAMS_INVALID);
    }
    else
    {
        response->fail(serviceResult.data);
    }

    return response;
}
