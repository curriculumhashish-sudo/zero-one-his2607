#pragma once
#ifndef _MEDICALRECORDCONTROLLER_H_
#define _MEDICALRECORDCONTROLLER_H_
#include "../../domain/GlobalInclude.h"
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
#include "../../domain/vo/inpatient-record/MedicalRecordVO.h"
//#include "../../../lib-oatpp/include/domain/query/PageQuery.h"
#include "../../domain/query/inpatient-record/MedicalRecordQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class MedicalRecordController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(MedicalRecordController);
public:
	//定义获取历史病历列表（条件+分页）接口描述
	ENDPOINT_INFO(queryBrief)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("inpatient-record.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("inpatient-record.summary1");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(ByTitlePageJsonVO);
		//分页查询参数
		API_DEF_ADD_PAGE_PARAMS();
		//病历名称查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "title", ZH_WORDS_GETTER("inpatient-record.title"), "", false);
	}
	//定义获取历史病历列表（条件+分页）接口处理
	ENDPOINT("GET", "/inpatient-record/brief", queryBrief, QUERIES(QueryParams, params),API_HANDLER_AUTH_PARAME)
	{
		//解析参数
		API_HANDLER_QUERY_PARAM(query, MedicalRecordQueryByTitle, params);
		//返回前端结果
		return createDtoResponse(Status::CODE_200, executeQueryByTitle(query));
		//API_HANDLER_RESP_VO(executeQueryTitle(query));
	}



	//API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("inpatient-record.summary1"), queryBrief, MedicalRecordQueryByTitle, ByTitlePageJsonVO::Wrapper, ZH_WORDS_GETTER("inpatient-record.api-name"));

	//API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/inpatient-record/brief", queryBrief, MedicalRecordQueryByTitle, executeQueryByTitle(query));



	//定义获取历史病历详情接口描述
	ENDPOINT_INFO(queryDetail)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("inpatient-record.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("inpatient-record.summary2");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(ByIdJsonVO);
		//病历id查询参数
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("inpatient-record.id"), "", true);
	}
	//定义获取历史病历详情接口处理
	ENDPOINT("GET", "/inpatient-record/detail", queryDetail, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME)
	{
		//解析参数
		API_HANDLER_QUERY_PARAM(query, MedicalRecordQueryById, params);
		//返回前端结果
		return createDtoResponse(Status::CODE_200, executeQueryById(query));
	}

	//定义保存病历接口描述
	ENDPOINT_INFO(addMedicalRecord)
	{
		//接口归属
		info->addTag(ZH_WORDS_GETTER("inpatient-record.api-name"));
		//授权描述
		API_DEF_ADD_AUTH();
		//接口标题
		info->summary = ZH_WORDS_GETTER("inpatient-record.summary5");
		//响应格式
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	}
	//定义保存病历接口处理
	ENDPOINT("POST", "/inpatient-record/add", addMedicalRecord, BODY_DTO(MedicalRecordAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME)
	{
		//返回前端结果
		return createDtoResponse(Status::CODE_200, execAddMedicalRecord(dto, authObject->getPayload()));
	}

	//ENDPOINT("POST", "/inpatient-record/add", addMedicalRecord, BODY_DTO(MedicalRecordAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME)
	//{
	//	OATPP_LOGD("ADD_DEBUG", "dto空指针? %s", dto ? "false 解析成功" : "true JSON解析失败");
	//	return createDtoResponse(Status::CODE_200, execAddMedicalRecord(dto, authObject->getPayload()));
	//}

	////定义保存病历接口2描述
	//ENDPOINT_INFO(modifyMedicalRecord)
	//{
	//	//接口归属
	//	info->addTag(ZH_WORDS_GETTER("inpatient-record.api-name"));
	//	//授权描述
	//	API_DEF_ADD_AUTH();
	//	//接口标题
	//	info->summary = ZH_WORDS_GETTER("inpatient-record.summary4");
	//	//响应格式
	//	API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	//}
	////定义保存病历接口2处理
	//ENDPOINT("PUT", "/inpatient-record/modify", modifyMedicalRecord, BODY_DTO(MedicalRecordModifyDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME)
	//{
	//	//返回前端结果
	//	return createDtoResponse(Status::CODE_200, execModifyMedicalRecord(dto, authObject->getPayload()));
	//}

private:
	ByTitlePageJsonVO::Wrapper executeQueryByTitle(const MedicalRecordQueryByTitle::Wrapper& query);
	ByIdJsonVO::Wrapper executeQueryById(const MedicalRecordQueryById::Wrapper& query);
	StringJsonVO::Wrapper execAddMedicalRecord(const MedicalRecordAddDTO::Wrapper& dto, const PayloadDTO& payload);
	//StringJsonVO::Wrapper execModifyMedicalRecord(const MedicalRecordModifyDTO::Wrapper& dto, const PayloadDTO& payload);
};

#include OATPP_CODEGEN_END(ApiController)
#endif