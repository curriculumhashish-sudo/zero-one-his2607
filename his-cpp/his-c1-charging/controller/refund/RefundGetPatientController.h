#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

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
#ifndef _REFUND_GET_PATIENT_CONTROLLER_
#define _REFUND_GET_PATIENT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/refund/RefundGetPatientQuery.h"
#include "domain/dto/refund/RefundGetPatientDTO.h"
#include "domain/vo/refund/RefundGetPatientVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// API tag for Swagger doc grouping: "Refund Management"
#define API_TAG ZH_WORDS_GETTER("charging.refund.tag")

class RefundGetPatientController : public oatpp::web::server::api::ApiController
{
    // JWT authorization - auto extracts token from header, verifies, and injects payload
    API_ACCESS_DECLARE(RefundGetPatientController);

public:
    // Endpoint metadata definition for Swagger documentation
    // POST /charging/refund/get-patient-list — paginated query for refund patient queue
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("xbzml.charging.refund.get-patient-list.summary"),
        queryAll,
        RefundGetPatientQuery,
        RefundGetPatientPageJsonVO::Wrapper,
        API_TAG);

    // Endpoint route binding
    // GET method with query params deserialized into RefundGetPatientQuery
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/refund/get-patient-list",
        queryAll,
        RefundGetPatientQuery,
        execQueryAll(query));

private:
    // Execute paginated query — delegates to Service layer
    RefundGetPatientPageJsonVO::Wrapper execQueryAll(
        const RefundGetPatientQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_REFUND_GET_PATIENT_CONTROLLER_
