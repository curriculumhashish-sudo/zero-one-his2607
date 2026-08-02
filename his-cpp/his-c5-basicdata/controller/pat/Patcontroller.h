#pragma once

#ifndef _PAT_CONTROLLER_
#define _PAT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/pat/PatVO.h"
#include "domain/dto/pat/PatDTO.h"
#include "domain/query/pat/PatQuery.h"
#include "domain/query/pat/PatPageQuery.h"
#include "domain/query/pat/PatSelectPageQuery.h"
#include "domain/vo/pat/PatPageJsonVO.h"
#include "domain/vo/pat/PatSelectPageJsonVO.h"
#include "ApiHelper.h"

// 1. 开启 API 控制器代码生成宏
#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("pat.tag")
/**
 * 患者管理控制器
 */
class PatController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatController);

public:
	// ================= 1. 分页查询患者接口 =================
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("pat.page.summary"), queryPatPage, PatPageQuery, PatPageJsonVO::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/pat/page", queryPatPage, PatPageQuery, execQueryPatPage(query, authObject->getPayload()));

	// ================= 2. 分页查询选择患者接口 =================
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("pat.select.page.summary"), querySelectPatPage, PatSelectPageQuery, PatSelectPageJsonVO::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/pat/select/page", querySelectPatPage, PatSelectPageQuery, execQuerySelectPatPage(query, authObject->getPayload()));

	// ================= 3. 删除患者接口 =================
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("pat.delete.summary"), removePat, ListJsonVO<String>::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/pat/delete", removePat, BODY_DTO(List<String>, ids), execRemovePat(ids));

private:
	// 业务处理私有函数声明（必须与 .cpp 中的实现一一对应）
	PatPageJsonVO::Wrapper execQueryPatPage(const PatPageQuery::Wrapper& query, const PayloadDTO& payload);
	PatSelectPageJsonVO::Wrapper execQuerySelectPatPage(const PatSelectPageQuery::Wrapper& query, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemovePat(const List<String>& ids);
};

#undef API_TAG

// 2. 关闭 API 控制器代码生成宏
#include OATPP_CODEGEN_END(ApiController)

#endif // _PAT_CONTROLLER_