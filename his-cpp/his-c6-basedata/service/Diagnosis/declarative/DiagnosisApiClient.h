#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/27

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
#ifndef _DIAGNOSIS_API_CLIENT_H_
#define _DIAGNOSIS_API_CLIENT_H_

#include "oatpp/web/client/ApiClient.hpp"
#include "ApiHelper.h"

/**
 * Declarative HTTP client for diagnosis catalog query services.
 */
class DiagnosisApiClient : public oatpp::web::client::ApiClient
{
#include OATPP_CODEGEN_BEGIN(ApiClient)

	API_CLIENT_INIT(DiagnosisApiClient);

	API_CALL(
		API_M_GET,
		"diagnosis/query-one",
		queryById,
		API_HANDLER_AUTN_HEADER,
		QUERY(Int64, id));

	API_CALL(
		API_M_GET,
		"diagnosis/query-all",
		queryAll,
		API_HANDLER_AUTN_HEADER,
		API_HANDLER_PAGE_PARAME,
		QUERY(String, keyword),
		QUERY(String, sourceEnum));

#include OATPP_CODEGEN_END(ApiClient)
};

#endif // !_DIAGNOSIS_API_CLIENT_H_
