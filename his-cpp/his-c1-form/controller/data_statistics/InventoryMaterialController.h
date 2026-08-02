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
#include "domain/dto/data_statistics/InventoryMaterialDto.h"
#include "ApiHelper.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

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

		// 整合了本地与云端的完整查询参数列表
		API_DEF_ADD_QUERY_PARAMS(String, "bus_no", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.bus_no"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "time", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.time"), "", false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "numbers", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.numbers"), 0.0, false);
		API_DEF_ADD_QUERY_PARAMS(String, "batch_number", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number"), "", false);
	}

	// Query endpoint handler
	ENDPOINT(API_M_GET, "/form/data_statistics/InventoryMaterial", queryObject1, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		// Bind query parameters to query DTO
		API_HANDLER_QUERY_PARAM(objectQuery1, InventoryMaterialQuery, queryParams);
		// Execute and respond
		API_HANDLER_RESP_VO(execQueryObject1(objectQuery1));
	}

	ENDPOINT_INFO(queryInventoryMaterialId) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("Litguidyo.form.inoutbound.query-by-id"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(InventoryMaterialVo);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_QUERY_PARAMS(
			String,
			"id",
			ZH_WORDS_GETTER("Litguidyo.form.id"),
			"900001",
			true
		);
	}

	API_HANDLER_ENDPOINT_AUTH(
		API_M_GET,
		"/form/data_statistics/InventoryMaterial/query-by-id",
		queryInventoryMaterialId,
		QUERY(String, id),
		execQueryById(id)
	);

private:
	// Paged query handler
	InventoryMaterialPageJsonVo::Wrapper execQueryObject1(const InventoryMaterialQuery::Wrapper& query);

	// Query by ID handler (保留云端新增的方法声明)
	InventoryMaterialVo::Wrapper execQueryById(const String& id);
};

// 保留防污染的宏清理
#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // !_INVENTORYCONTROLLER_H_