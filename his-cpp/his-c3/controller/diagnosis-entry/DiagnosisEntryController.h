#pragma once
#pragma once
#ifndef _DIAGNOSISENTRYCONTROLLER_H_
#define _DIAGNOSISENTRYCONTROLLER_H_
#include "../../domain/GlobalInclude.h"
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
#include "../../domain/vo/diagnosis-entry/DiagnosisEntryVO.h"
#include "../../domain/query/diagnosis-entry/DiagnosisEntryQuery.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
#include OATPP_CODEGEN_BEGIN(ApiController)

class DiagnosisEntryController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnosisEntryController);
public:
	//定义获取诊断使用历史接口描述
	ENDPOINT_INFO(queryBrief)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("diagnosis-entry.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("diagnosis-entry.summary1");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(DiagnosisEntryPageJsonVO);
		//租户id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "tenant_id", ZH_WORDS_GETTER("diagnosis-entry.tenant_id"), "", true);

	}
	//定义获取诊断使用历史接口处理
	ENDPOINT("GET", "/diagnosis-entry/brief", queryBrief, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME)
	{
		//解析参数
		API_HANDLER_QUERY_PARAM(query, DiagnosisEntryQueryNull, params);
		//返回前端结果
		return createDtoResponse(Status::CODE_200, executeQueryBrief(query));
	}

	//定义获取患者诊断记录（条件+分页）接口描述
	ENDPOINT_INFO(queryDetail)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("diagnosis-entry.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("diagnosis-entry.summary2");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(DiagnosisEntryDetailPageJsonVO);
		//分页查询参数
		API_DEF_ADD_PAGE_PARAMS();
		//租户id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "tenant_id", ZH_WORDS_GETTER("diagnosis-entry.tenant_id"), "", true);
		//患者id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "patient_id", ZH_WORDS_GETTER("diagnosis-entry.patient-id"), "", true);
		//患者id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "encounter_id", ZH_WORDS_GETTER("diagnosis-entry.encounter_id"), "", true);
	}
	//定义获取患者诊断记录（条件+分页）接口处理
	ENDPOINT("GET", "/diagnosis-entry/detail", queryDetail, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME)
	{
		//解析参数
		API_HANDLER_QUERY_PARAM(query, DiagnosisEntryQueryById, params);
		//返回前端结果
		return createDtoResponse(Status::CODE_200, executeQueryDetail(query));
	}


private:
	DiagnosisEntryPageJsonVO::Wrapper executeQueryBrief(const DiagnosisEntryQueryNull::Wrapper& query);
	DiagnosisEntryDetailPageJsonVO::Wrapper executeQueryDetail(const DiagnosisEntryQueryById::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController)
#endif