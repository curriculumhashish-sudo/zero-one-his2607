#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
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
#ifndef _INVENTORYCONTROLLER_H_
#define _INVENTORYCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/data_statistics/InventoryMaterialQuery.h"
#include "domain/vo/data_statistics/InventoryMaterialVo.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("form.data_statistics.tag")

class InventoryMaterialController : public oatpp::web::server::api::ApiController {
	// Declare auth access handler
	API_ACCESS_DECLARE(InventoryMaterialController);

public:
	ENDPOINT_INFO(queryObject1) {
		// Set endpoint title
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("Litguidyo.form.inoutbound.summary2"));
		// Enable default auth
		API_DEF_ADD_AUTH();
		// Set response JSON wrapper type
		API_DEF_ADD_RSP_JSON_WRAPPER(InventoryMaterialPageJsonVo);
		// Set tag
		API_DEF_ADD_TAG(API_TAG);
		// Set paged query parameters
		API_DEF_ADD_PAGE_PARAMS();
		// Set query filter parameters (aligned with InventoryMaterialQuery DTO fields)
		API_DEF_ADD_QUERY_PARAMS(String, "time", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.time"), "", false);
		API_DEF_ADD_QUERY_PARAMS(Int32, "numbers", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.numbers"), 0, false);
		API_DEF_ADD_QUERY_PARAMS(String, "batch_number", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number"), "", false);
	}

	// Query endpoint handler
	ENDPOINT(API_M_GET, "/form/data_statistics/InventoryMaterial", queryObject1, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// Bind query parameters to query DTO
		API_HANDLER_QUERY_PARAM(objectQuery1, InventoryMaterialQuery, queryParams);
		// Execute and respond
		API_HANDLER_RESP_VO(execQueryObject1(objectQuery1));
	}

private:
	// Paged query handler
	InventoryMaterialPageJsonVo::Wrapper execQueryObject1(const InventoryMaterialQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // !_INVENTORYCONTROLLER_H_
