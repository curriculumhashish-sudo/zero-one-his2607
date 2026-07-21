#pragma once

#ifndef _CHINAPRESCONTROLLER_
#define _CHINAPRESCONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include"domain/query/chinapres/ChinapresQuery.h"
#include"domain/query/chinapres/ChinapresDetailQuery.h"
#include"domain/vo/chinapres/ChinapresVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController) 
#define API_TAG ZH_WORDS_GETTER("chinapres.tags.tag")

class ChinapresController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(ChinapresController);
public:

	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("chinapres.query.summary"), queryChinapres, ChinapresQuery, ChinapresPageJsonVO::Wrapper, API_TAG);
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("chinapres.detail.summary"), queryChinapresdetails, ChinapresDetailQuery, ChinapresDetailJsonVO::Wrapper, API_TAG);
	//响应
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chinapres", queryChinapres, ChinapresQuery, executeQueryChinapres(query));
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chinapres-datail", queryChinapresdetails, ChinapresDetailQuery, executeQueryChinapresDetail(query));
private:
	ChinapresPageJsonVO::Wrapper executeQueryChinapres(const ChinapresQuery::Wrapper& query);
	ChinapresDetailJsonVO::Wrapper executeQueryChinapresDetail(const ChinapresDetailQuery::Wrapper&query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController) 

#endif // _CHINAPRESCONTROLLER_