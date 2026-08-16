#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 Declares business services used by TCM prescription actions.

 @Author: nick1484065161
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

// Prevents duplicate service declarations when this header is included repeatedly.
#ifndef _TCM_PRESCRIPTION_SERVICE_H_
#define _TCM_PRESCRIPTION_SERVICE_H_

#include "domain/dto/PayloadDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionSignDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionSignResultDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionWithdrawDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionWithdrawResultDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionStopDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionStopResultDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionCancelStopDTO.h"
#include "../../domain/dto/prescription/TcmPrescriptionCancelStopResultDTO.h"

/**
 * Carries the internal outcome of signing a prescription.
 */
struct TcmPrescriptionSignServiceResult
{
    // Indicates whether the business action completed successfully.
    bool success = false;

    // Indicates whether the failure was caused by invalid request data.
    bool invalidRequest = false;

    // Describes the business outcome for the controller.
    std::string message;

    // Contains the response data returned by the signing action.
    TcmPrescriptionSignResultDTO::Wrapper data;
};

/**
 * Carries the internal outcome of a prescription withdrawal.
 */
struct TcmPrescriptionWithdrawServiceResult
{
    // Indicates whether the business action completed successfully.
    bool success = false;
    // Indicates whether the failure was caused by invalid request data.
    bool invalidRequest = false;
    // Describes the business outcome for the controller.
    std::string message;
    // Contains the response data returned by the withdrawal action.
    TcmPrescriptionWithdrawResultDTO::Wrapper data;
};

/**
 * Carries the internal outcome of stopping a prescription.
 */
struct TcmPrescriptionStopServiceResult
{
    // Indicates whether the business action completed successfully.
    bool success = false;
    // Indicates whether the failure was caused by invalid request data.
    bool invalidRequest = false;
    // Describes the business outcome for the controller.
    std::string message;
    // Contains the response data returned by the stop action.
    TcmPrescriptionStopResultDTO::Wrapper data;
};

/**
 * Carries the internal outcome of canceling a prescription stop.
 */
struct TcmPrescriptionCancelStopServiceResult
{
    // Indicates whether the business action completed successfully.
    bool success = false;
    // Indicates whether the failure was caused by invalid request data.
    bool invalidRequest = false;
    // Describes the business outcome for the controller.
    std::string message;
    // Contains the response data returned by the cancel-stop action.
    TcmPrescriptionCancelStopResultDTO::Wrapper data;
};

/**
 * Coordinates validation, transactions, and database updates for TCM
 * prescription state changes.
 */
class TcmPrescriptionService
{
public:
    // Signs eligible draft prescriptions and assigns numbers by group.
    TcmPrescriptionSignServiceResult signPrescription(
        const TcmPrescriptionSignDTO::Wrapper& dto,
        const PayloadDTO& payload
    );

    // Withdraws an eligible prescription and returns its business outcome.
    TcmPrescriptionWithdrawServiceResult withdrawPrescription(
        const TcmPrescriptionWithdrawDTO::Wrapper& dto,
        const PayloadDTO& payload
    );

    // Stops an eligible prescription at the requested effective time.
    TcmPrescriptionStopServiceResult stopPrescription(
        const TcmPrescriptionStopDTO::Wrapper& dto,
        const PayloadDTO& payload
    );

    // Cancels a previous prescription stop and restores its active state.
    TcmPrescriptionCancelStopServiceResult cancelStopPrescription(
        const TcmPrescriptionCancelStopDTO::Wrapper& dto,
        const PayloadDTO& payload
    );
};

#endif // !_TCM_PRESCRIPTION_SERVICE_H_
