#pragma once
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
	API_ACCESS_DECLARE(InventoryMaterialController);

public:
	ENDPOINT_INFO(queryObject1) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("Litguidyo.form.inoutbound.summary2"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(InventoryMaterialPageJsonVo);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PAGE_PARAMS();
		API_DEF_ADD_QUERY_PARAMS(String, "bus_no", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.bus_no"), "", false);
		API_DEF_ADD_QUERY_PARAMS(String, "time", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.time"), "", false);
		API_DEF_ADD_QUERY_PARAMS(Float64, "numbers", ZH_WORDS_GETTER("Litguidyo.form.inoutbound.numbers"), 0.0, false);
	}

	ENDPOINT(API_M_GET, "/form/data_statistics/InventoryMaterial", queryObject1, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_QUERY_PARAM(objectQuery1, InventoryMaterialQuery, queryParams);
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
	InventoryMaterialPageJsonVo::Wrapper execQueryObject1(const InventoryMaterialQuery::Wrapper& query);
	InventoryMaterialVo::Wrapper execQueryById(const String& id);
};

#include OATPP_CODEGEN_END(ApiController)

#endif // !_INVENTORYCONTROLLER_H_
