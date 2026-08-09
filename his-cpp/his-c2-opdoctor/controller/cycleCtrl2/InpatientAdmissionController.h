#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/08/08 10:00:00

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
#ifndef _INPATIENTADMISSIONCONTROLLER_H_
#define _INPATIENTADMISSIONCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/cycleCtrl2-admission/InpatientAdmissionWardsQuery.h"
#include "domain/vo/cycleCtrl2-admission/InpatientAdmissionWardVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG2 ZH_WORDS_GETTER("cyclectrl2.test.tag")

class InpatientAdmissionController : public oatpp::web::server::api::ApiController
{
	// Endpoint access declaration
	API_ACCESS_DECLARE(InpatientAdmissionController);
public:
	// 3.1.3 Load the wards of a target inpatient department
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("cyclectrl2.admission.listWards"),
		listInpatientAdmissionWards, InpatientAdmissionWardListJsonVO::Wrapper, API_TAG2);
	// 3.1.4 Handle the ward list request
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/doctor-station/inpatient-admission/wards",
		listInpatientAdmissionWards, BODY_DTO(InpatientAdmissionWardsQuery::Wrapper, query),
		execListInpatientAdmissionWards(query));
private:
	// Execute the ward list request
	InpatientAdmissionWardListJsonVO::Wrapper execListInpatientAdmissionWards(
		const InpatientAdmissionWardsQuery::Wrapper& query);
};
#undef API_TAG2

#include OATPP_CODEGEN_END(ApiController)
#endif // !_INPATIENTADMISSIONCONTROLLER_H_