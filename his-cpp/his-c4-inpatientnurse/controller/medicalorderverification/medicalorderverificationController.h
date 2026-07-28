#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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

#ifndef _MEDICALORDERVERIFICATIONCONTROLLER_H_
#define _MEDICALORDERVERIFICATIONCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/medicalorderverification/medicalorderverificationVO.h"
#include "domain/vo/medicalorderverification/VerificationVO.h"
#include "domain/query/medicalorderverification/medicalorderverificationQuery.h"
#include "domain/dto/medicalorderverification/VerificationDTO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medicalOrderVerification.tags.tag")

class MedicalOrderVerificationController : public oatpp::web::server::api::ApiController
{
    // Declare API access control
    API_ACCESS_DECLARE(MedicalOrderVerificationController);
public:
    // Endpoint info definition
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medicalOrderVerification.query.summary"),
                                      queryMedicalOrderVerification,
                                      medicalOrderVerificationJsonVO::Wrapper,
                                      API_TAG,
                                      API_DEF_QUERY_PARAM_BUILD(MedicalVerificationQuery);
                                      info->responses[Status::CODE_200].addExample(
                                          "success", medicalOrderVerificationJsonVO::createShared());
                                      );

    // Endpoint binding
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/medicalOrderVerification/query",
                                    queryMedicalOrderVerification,
                                    MedicalVerificationQuery,
                                    execQueryMedicalOrderVerification(query));

    // Batch verify orders pass
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medicalOrderVerification.verify-pass.summary"),
        verifyPass,
        VerifyPassJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", VerifyPassRequestDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", VerifyPassJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medicalOrderVerification/verify-pass",
        verifyPass,
        BODY_DTO(VerifyPassRequestDTO::Wrapper, dto),
                                    execVerifyPass(dto, authObject->getPayload()));

    // Batch reject orders
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medicalOrderVerification.reject-orders.summary"),
        rejectOrders,
        RejectOrdersJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", RejectOrdersRequestDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", RejectOrdersJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medicalOrderVerification/reject-orders",
        rejectOrders,
        BODY_DTO(RejectOrdersRequestDTO::Wrapper, dto),
        execRejectOrders(dto, authObject->getPayload()));

private:
    // Endpoint execution function
    medicalOrderVerificationJsonVO::Wrapper execQueryMedicalOrderVerification(
        const MedicalVerificationQuery::Wrapper& query);
    VerifyPassJsonVO::Wrapper execVerifyPass(const VerifyPassRequestDTO::Wrapper& dto,const PayloadDTO& payload);
    RejectOrdersJsonVO::Wrapper execRejectOrders(const RejectOrdersRequestDTO::Wrapper& dto,const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICALORDERVERIFICATIONCONTROLLER_H_
