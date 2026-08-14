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
#pragma once

// Prevents duplicate controller declarations when this header is included repeatedly.
#ifndef _TCM_PRESCRIPTION_CONTROLLER_H_
#define _TCM_PRESCRIPTION_CONTROLLER_H_

#include "stdafx.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/prescription/TcmPrescriptionSignDTO.h"
#include "domain/vo/prescription/TcmPrescriptionSignVO.h"
#include "domain/dto/prescription/TcmPrescriptionWithdrawDTO.h"
#include "domain/vo/prescription/TcmPrescriptionWithdrawVO.h"

#include "domain/dto/prescription/TcmPrescriptionStopDTO.h"
#include "domain/vo/prescription/TcmPrescriptionStopVO.h"
#include "domain/dto/prescription/TcmPrescriptionCancelStopDTO.h"
#include "domain/vo/prescription/TcmPrescriptionCancelStopVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// Defines the Swagger tag shared by all TCM prescription endpoints.
#define API_TAG ZH_WORDS_GETTER("tcm-prescription.tags.tag")

/**
 * Exposes authenticated TCM prescription state-transition endpoints.
 */
class TcmPrescriptionController : public oatpp::web::server::api::ApiController
{
    // Registers the controller with the authenticated API infrastructure.
    API_ACCESS_DECLARE(TcmPrescriptionController);

  public:
    // Describes the authenticated prescription-sign endpoint for Swagger.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.sign.summary"), signTcmPrescription,
                               TcmPrescriptionSignJsonVO::Wrapper, API_TAG);

    // Binds POST /tcm-prescription/sign to the sign handler.
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/sign", signTcmPrescription,
                              BODY_DTO(TcmPrescriptionSignDTO::Wrapper, dto),
                              execSignTcmPrescription(dto, authObject->getPayload()));

    // Describes the authenticated prescription-withdrawal endpoint for Swagger.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.withdraw.summary"), withdrawTcmPrescription,
                               TcmPrescriptionWithdrawJsonVO::Wrapper, API_TAG);

    // Binds POST /tcm-prescription/withdraw to the withdrawal handler.
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/withdraw", withdrawTcmPrescription,
                              BODY_DTO(TcmPrescriptionWithdrawDTO::Wrapper, dto),
                              execWithdrawTcmPrescription(dto, authObject->getPayload()));

    // Describes the authenticated prescription-stop endpoint for Swagger.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.stop.summary"), stopTcmPrescription,
                               TcmPrescriptionStopJsonVO::Wrapper, API_TAG);

    // Binds POST /tcm-prescription/stop to the stop handler.
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/stop", stopTcmPrescription,
                              BODY_DTO(TcmPrescriptionStopDTO::Wrapper, dto),
                              execStopTcmPrescription(dto, authObject->getPayload()));

    // Describes the authenticated cancel-stop endpoint for Swagger.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.cancel-stop.summary"), cancelStopTcmPrescription,
                               TcmPrescriptionCancelStopJsonVO::Wrapper, API_TAG);

    // Binds POST /tcm-prescription/cancel-stop to the cancel-stop handler.
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/cancel-stop", cancelStopTcmPrescription,
                              BODY_DTO(TcmPrescriptionCancelStopDTO::Wrapper, dto),
                              execCancelStopTcmPrescription(dto, authObject->getPayload()));

  private:
    // Validates and delegates a prescription-stop request.
    TcmPrescriptionStopJsonVO::Wrapper execStopTcmPrescription(const TcmPrescriptionStopDTO::Wrapper &dto,
                                                               const PayloadDTO &payload);

    // Validates and delegates a cancel-stop request.
    TcmPrescriptionCancelStopJsonVO::Wrapper execCancelStopTcmPrescription(
        const TcmPrescriptionCancelStopDTO::Wrapper &dto, const PayloadDTO &payload);

    // Validates and delegates a prescription-signing request.
    TcmPrescriptionSignJsonVO::Wrapper execSignTcmPrescription(const TcmPrescriptionSignDTO::Wrapper &dto,
                                                               const PayloadDTO &payload);

    // Validates and delegates a prescription-withdrawal request.
    TcmPrescriptionWithdrawJsonVO::Wrapper execWithdrawTcmPrescription(const TcmPrescriptionWithdrawDTO::Wrapper &dto,
                                                                       const PayloadDTO &payload);
};

// Removes the controller-local Swagger tag macro after endpoint generation.
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

// Ends the TCM prescription controller include guard.
#endif // !_TCM_PRESCRIPTION_CONTROLLER_H_
