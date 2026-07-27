#pragma once

#ifndef _DRUGSERVICE_
#define _DRUGSERVICE_

// 1 絳斛剕芛恅璃
#include "oatpp/web/client/ApiClient.hpp"
#include "ApiHelper.h"

/**
 * 抎迡尨瞰ApiClient懂栳尨隅砱汒隴宒督昢
 */
class DrugApiClient : public oatpp::web::client::ApiClient // 2 樟創涴跺濬
{
	// 3 隅砱ApiClient諷秶妏蚚粽---羲宎
#include OATPP_CODEGEN_BEGIN(ApiClient)

	// 4 場宎趙隅砱
	API_CLIENT_INIT(DrugApiClient);

	// 5 妏蚚API_CALL麼API_CALL_ASYNC汒隴覃蚚督昢諉諳

	// 籵徹ID脤戙
	API_CALL(API_M_GET, "Drug/query-one", queryById, API_HANDLER_AUTN_HEADER, QUERY(String, id));
	// 嗣沭璃脤戙
	API_CALL(API_M_GET, "Drug/query-all", queryAll, API_HANDLER_AUTN_HEADER, API_HANDLER_PAGE_PARAME, QUERY(String, name));

#include OATPP_CODEGEN_END(ApiClient)
	// 3 秏ApiClient諷秶妏蚚粽---賦旰
};

#endif // !_