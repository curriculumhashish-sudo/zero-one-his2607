#pragma once
#ifndef  _DRUGCONTROLLER_H_
#define _DRUGCONTROLLER_H_
#include "domain/vo/BaseJsonVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)

#include "domain/query/Drug/DrugQuery.h"
#include "domain/vo/drug/DrugListVO.h"
#include "domain/dto/drug/DrugAddDTO.h"
#include "domain/GlobalInclude.h"
#include "domain/vo/NoDataJsonVO.h"
#define API_TAG ZH_WORDS_GETTER("medication.tag")
class DrugController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DrugController);
public:
	// 分页查询接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("medication.query.summary"), queryDrug, DrugQuery, DrugListPageJsonVO::Wrapper, API_TAG);
	// 分页接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/Drug", queryDrug,  DrugQuery, executeQueryDrug(query));

	// 3.1 新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.add.summary"), addDrug, StringJsonVO::Wrapper, API_TAG);
	// 3.2 新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/Drug/add", addDrug, BODY_DTO(DrugAddDTO::Wrapper, dto), execAddDrug(dto, authObject->getPayload()));
	// 定义获取用户详细信息接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medication.query-one.summary"), queryOneEquipment, DrugDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("drug.field.id"), "ae65c714d48d4f34b52479f5482c0edd", true);
	);
	// 定义获取用户详细信息接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/drug/query-one", queryOneEquipment, QUERY(String, id), executeQueryOne(id));

private:
	//执行函数接口
	DrugListPageJsonVO::Wrapper executeQueryDrug(const DrugQuery::Wrapper& query);
	StringJsonVO::Wrapper  execAddDrug(const DrugAddDTO::Wrapper& dto, const PayloadDTO& payload);
	DrugDetailJsonVO::Wrapper executeQueryOne(const String& id);
};
#include OATPP_CODEGEN_END(ApiController)

#endif 