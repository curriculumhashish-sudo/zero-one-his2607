#pragma once

#ifndef _PAT_CONTROLLER_
#define _PAT_CONTROLLER_

// 框架基础VO（3层../到项目根）
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
// 患者分页Query
#include "../../domain/query/pat/PatPageQuery.h"
#include "../../domain/query/pat/PatSelectPageQuery.h"
// 分页外层VO
#include "../../domain/vo/pat/PatPageJsonVO.h"
#include "../../domain/vo/pat/PatSelectPageJsonVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG_PAT ZH_WORDS_GETTER("pat.tag")

/**
 * 患者管理控制器
 */
class PatController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatController);
public:
	// 分页查询患者接口（专用分页宏 API_DEF_ENDPOINT_INFO_QUERY_AUTH）
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("pat.page.summary"), queryPatPage, PatPageQuery, PatPageJsonVO::Wrapper, API_TAG_PAT
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/pat/page", queryPatPage, PatPageQuery, execQueryPatPage(query, authObject->getPayload()));

	//分页查询选择患者接口
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("pat.select.page.summary"), querySelectPatPage, PatSelectPageQuery, PatSelectPageJsonVO::Wrapper, API_TAG_PAT
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/pat/select/page", querySelectPatPage, PatSelectPageQuery, execQuerySelectPatPage(query, authObject->getPayload()));

private:
	// 分页查询业务处理函数
	PatPageJsonVO::Wrapper execQueryPatPage(const PatPageQuery::Wrapper& query, const PayloadDTO& payload);

	PatSelectPageJsonVO::Wrapper execQuerySelectPatPage(const PatSelectPageQuery::Wrapper& query, const PayloadDTO& payload);
};

#undef API_TAG_PAT
#include OATPP_CODEGEN_END(ApiController)
#endif // _PAT_CONTROLLER_