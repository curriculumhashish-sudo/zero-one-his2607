#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author:
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
#include "domain/query/medicalorderverification/medicalorderverificationQuery.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medicalOrderVerification.tags.tag")

class MedicalOrderVerificationController : public oatpp::web::server::api::ApiController
{
    // Declare API access control
    API_ACCESS_DECLARE(MedicalOrderVerificationController);
public:
    // Endpoint info definition
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("medicalOrderVerification.query.summary"),
                                      queryMedicalOrderVerification,
                                      MedicalVerificationQuery,
                                      medicalOrderVerificationJsonVO::Wrapper,
                                      API_TAG);

    // Endpoint binding
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/medicalOrderVerification/query",
                                    queryMedicalOrderVerification,
                                    MedicalVerificationQuery,
                                    execQueryMedicalOrderVerification(query));

private:
    // Endpoint execution function
    medicalOrderVerificationJsonVO::Wrapper execQueryMedicalOrderVerification(
        const MedicalVerificationQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICALORDERVERIFICATIONCONTROLLER_H_ 