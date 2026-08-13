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
#include "domain/vo/medicalorderverification/MedicalOrderVerificationVO.h"
#include "domain/vo/medicalorderverification/VerificationVO.h"
#include "domain/query/medicalorderverification/MedicalOrderVerificationQuery.h"
#include "domain/dto/medicalorderverification/VerificationDTO.h"
#include "controller/RequestValidation.h"

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
                                      MedicalOrderVerificationJsonVO::Wrapper,
                                      API_TAG,
                                      API_DEF_QUERY_PARAM_BUILD(MedicalVerificationQuery);
                                      info->queryParams["name"].addExample("default", String("Zhang San"));
                                      info->queryParams["isPassed"].addExample("default", String("false"));
                                      info->queryParams["medicalType"].addExample("default", String("MED_REQUEST"));
                                      info->queryParams["medicalPeriod"].addExample("default", String("TEMPORARY"));
                                      info->queryParams["patientId"].addExample("default", String("326940849401761792"));
                                      info->queryParams["encounterId"].addExample("default", String("337907884548657152"));
                                      info->queryParams["locationId"].addExample("default", String("334526752078102528"));
                                      info->queryParams["medicineCategory"].addExample("default", String("WESTERN_MEDICINE"));
                                      info->queryParams["therapyEnum"].addExample("default", String("TEMPORARY"));
                                      info->queryParams["orderType"].addExample("default", String("MED_REQUEST"));
                                      info->queryParams["statusEnum"].addExample("default", String("ACTIVE"));
                                      info->responses[Status::CODE_200].addExample(
                                          "success", MedicalOrderVerificationJsonVO::createShared());
                                      );

    // Endpoint binding
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/medicalOrderVerification/query",
                                    queryMedicalOrderVerification,
                                    MedicalVerificationQuery,
                                    execQueryMedicalOrderVerification(
										query, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicalOrderVerification.query-workbench.summary"),
		queryMedicalOrderVerificationWorkbench,
		MedicalVerificationWorkbenchJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", MedicalVerificationWorkbenchQueryDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", MedicalVerificationWorkbenchJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/medicalOrderVerification/query",
		queryMedicalOrderVerificationWorkbench,
		C4_REQUIRED_BODY_DTO(MedicalVerificationWorkbenchQueryDTO::Wrapper, dto),
		execQueryMedicalOrderVerificationWorkbench(dto, authObject->getPayload()));

    // Batch verify orders pass
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medicalOrderVerification.verify-pass.summary"),
        verifyPass,
        VerifyPassJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", VerifyPassRequestDTO::createExample());
        info->responses[Status::CODE_200].addExample(
            "success", VerifyPassJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medicalOrderVerification/verify-pass",
        verifyPass,
        C4_REQUIRED_BODY_DTO(VerifyPassRequestDTO::Wrapper, dto),
                                    execVerifyPass(dto, authObject->getPayload()));

    // Batch reject orders
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("medicalOrderVerification.reject-orders.summary"),
        rejectOrders,
        RejectOrdersJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", RejectOrdersRequestDTO::createExample());
        info->responses[Status::CODE_200].addExample(
            "success", RejectOrdersJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/medicalOrderVerification/reject-orders",
        rejectOrders,
        C4_REQUIRED_BODY_DTO(RejectOrdersRequestDTO::Wrapper, dto),
        execRejectOrders(dto, authObject->getPayload()));

private:
    // Endpoint execution function
    MedicalOrderVerificationJsonVO::Wrapper execQueryMedicalOrderVerification(
        const MedicalVerificationQuery::Wrapper& query, const PayloadDTO& payload);
	MedicalVerificationWorkbenchJsonVO::Wrapper execQueryMedicalOrderVerificationWorkbench(
		const MedicalVerificationWorkbenchQueryDTO::Wrapper& dto,
		const PayloadDTO& payload);
    VerifyPassJsonVO::Wrapper execVerifyPass(const VerifyPassRequestDTO::Wrapper& dto,const PayloadDTO& payload);
    RejectOrdersJsonVO::Wrapper execRejectOrders(const RejectOrdersRequestDTO::Wrapper& dto,const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICALORDERVERIFICATIONCONTROLLER_H_
