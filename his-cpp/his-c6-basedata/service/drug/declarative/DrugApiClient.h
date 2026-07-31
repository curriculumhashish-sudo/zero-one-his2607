#pragma once

#ifndef _DRUG_API_CLIENT_H_
#define _DRUG_API_CLIENT_H_

#include "oatpp/web/client/ApiClient.hpp"
#include "ApiHelper.h"

/** 药品声明式服务客户端。 */
class DrugApiClient : public oatpp::web::client::ApiClient
{
#include OATPP_CODEGEN_BEGIN(ApiClient)

	API_CLIENT_INIT(DrugApiClient);

	// 声明式单条查询。
	API_CALL(API_M_GET, "drug/query-one", queryById, API_HANDLER_AUTN_HEADER, QUERY(String, id));
	// 声明式分页查询。
	API_CALL(API_M_GET, "drug/query-all", queryAll, API_HANDLER_AUTN_HEADER,
			 API_HANDLER_PAGE_PARAME, QUERY(String, keywords));

#include OATPP_CODEGEN_END(ApiClient)
};

#endif // !_DRUG_API_CLIENT_H_
