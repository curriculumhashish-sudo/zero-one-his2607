#pragma once
#ifndef  _DRUGCONTROLLER_H_
#define _DRUGCONTROLLER_H_
#include "domain/vo/BaseJsonVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)
#include "../../GlobalInclude.h"
#include "domain/query/Drug/DrugQuery.h"
#include "domain/vo/drug/DrugListVO.h"

#include "domain/GlobalInclude.h"
#include "domain/vo/NoDataJsonVO.h"
#define API_TAG ZH_WORDS_GETTER("medication.tag")
class DrugController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DrugController);
public:
	// 分页查询接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("Drug.query.summary"), queryDrug, DrugQuery, DrugListPageJsonVO::Wrapper, API_TAG);
	// 分页接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/Drug", queryDrug,  DrugQuery, executeQueryDrug(query));

private:
	//执行函数接口
	DrugListPageJsonVO::Wrapper executeQueryDrug(const DrugQuery::Wrapper& query);
};
#include OATPP_CODEGEN_END(ApiController)

#endif 