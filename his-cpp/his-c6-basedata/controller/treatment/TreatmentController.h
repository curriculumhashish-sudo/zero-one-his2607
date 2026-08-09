#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/19 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _TREATMENTCONTROLLER_H_
#define _TREATMENTCONTROLLER_H_


#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/treatment/TreatmentVO.h"
#include "domain/vo/treatment/TreatmentOptionsVO.h"
#include "domain/dto/treatment/TreatmentAddDTO.h"
#include "domain/dto/treatment/TreatmentModifyDTO.h"
#include "domain/query/treatment/TreatmentQuery.h"
#include "domain/query/treatment/TreatmentOptionsQuery.h"
#include "domain/dto/treatment/TreatmentPageDTO.h"
#include "domain/dto/treatment/TreatmentStatusDTO.h"
#include "domain/dto/treatment/TreatmentRemoveDTO.h"

#include <algorithm>
#include <charconv>
#include <exception>
#include <system_error>

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("treatment.tags.tag")
#define API_TAG2 ZH_WORDS_GETTER("treatment.tags.tag2")
class TreatmentController : public oatpp::web::server::api::ApiController
{
	// 权限相关声明
	API_ACCESS_DECLARE(TreatmentController);
public: // 接口定义
	// 通过ID获取诊疗详情：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.query-by-id.summary"), queryTreatmentById,
		TreatmentDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("treatment.field.id"), "184000000000000001", true););
	// 通过ID获取诊疗详情：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/treatment/query-by-id", queryTreatmentById, QUERY(String, id), execQueryById(id));

	// 新增诊疗：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.add.summary"), addTreatment, StringJsonVO::Wrapper, API_TAG);
	// 新增诊疗：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/treatment/add-treatment", addTreatment,
		BODY_DTO(TreatmentAddDTO::Wrapper, dto), execAddTreatment(dto, authObject->getPayload()));

	// 修改诊疗：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.modify.summary"), modifyTreatment, StringJsonVO::Wrapper, API_TAG);
	// 修改诊疗：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/treatment/modify-treatment", modifyTreatment,
		BODY_DTO(TreatmentModifyDTO::Wrapper, dto), execModifyTreatment(dto, authObject->getPayload()));

	// 启/停用诊疗（支持批量）：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.modify-status.summary"), modifyTreatmentStatus, StringJsonVO::Wrapper, API_TAG);
	// 启/停用诊疗（支持批量）：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/treatment/modify-status", modifyTreatmentStatus,
		BODY_DTO(TreatmentStatusDTO::Wrapper, dto), execModifyStatus(dto, authObject->getPayload()));

	// 删除诊疗（支持批量）：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.remove.summary"), removeTreatment, ListJsonVO<String>::Wrapper, API_TAG);
	// 删除诊疗（支持批量）：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/treatment/remove-treatment", removeTreatment, BODY_DTO(List<String>, ids), execRemoveTreatment(ids));

	// 获取选择诊疗列表（条件+分页）。
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.options-v2.summary"), queryTreatmentOptionsV2,
		TreatmentOptionsPageJsonVO::Wrapper, API_TAG,
		API_DEF_QUERY_PARAM_BUILD(TreatmentOptionsQuery);
		info->queryParams["pageIndex"].addExample("default", UInt64(1));
		info->queryParams["pageSize"].addExample("default", UInt64(20));
		info->queryParams["keyword"].addExample("example", String("AD00000013"));
		info->queryParams["categoryCode"].addExample("example", String("22"));
		info->authorization = "Bearer";
		info->addResponse<TreatmentOptionsPageJsonVO::Wrapper>(Status::CODE_400, "application/json");
		info->addResponse<TreatmentOptionsPageJsonVO::Wrapper>(Status::CODE_401, "application/json");
		info->addResponse<TreatmentOptionsPageJsonVO::Wrapper>(Status::CODE_403, "application/json");
		info->addResponse<TreatmentOptionsPageJsonVO::Wrapper>(Status::CODE_500, "application/json"););
	ENDPOINT(API_M_GET, "/api/v2/treatments/options", queryTreatmentOptionsV2,
		QUERIES(QueryParams, queryParams),
		HEADER(String, authorization, API_H_TOKEN))
	{
		// 构造符合接口契约的统一错误响应。
		auto makeError = [](const char* key, int code)
		{
			auto result = TreatmentOptionsPageJsonVO::createShared();
			result->init(nullptr, ResultStatus(ZH_WORDS_GETTER(key), code));
			return result;
		};

		// 校验 Bearer 凭证并解析当前用户。
		std::shared_ptr<CustomerAuthorizeObject> v2AuthObject;
		try
		{
			auto authorizationHandler = ApiController::getDefaultAuthorizationHandler();
			if (authorizationHandler)
			{
				auto authorizationObject = authorizationHandler->handleAuthorization(authorization);
				v2AuthObject = std::dynamic_pointer_cast<CustomerAuthorizeObject>(authorizationObject);
			}
		}
		catch (const std::exception&)
		{
		}
		if (!v2AuthObject)
		{
			return createDtoResponse(Status::CODE_401,
				makeError("treatment.message.unauthorized", 10002));
		}

		// 校验诊疗选择列表访问权限。
		const auto& authorities = v2AuthObject->getPayload().getAuthorities();
		const bool permitted =
			std::find(authorities.begin(), authorities.end(), "SUPER_ADMIN") != authorities.end() ||
			std::find(authorities.begin(), authorities.end(),
				"Navigation.Diagnosistreatment") != authorities.end();
		if (!permitted)
		{
			return createDtoResponse(Status::CODE_403,
				makeError("treatment.message.forbidden", 10003));
		}

		// 解析并校验分页及筛选参数。
		auto query = TreatmentOptionsQuery::createShared();
		bool pageIndexSet = false;
		bool pageSizeSet = false;
		bool valid = true;
		for (const auto& param : queryParams.getAll())
		{
			const std::string name = param.first.toString();
			auto data = param.second.getMemoryHandle().get();
			if (!data)
			{
				valid = false;
				break;
			}
			const std::string value = *data;
			if (name == "pageIndex" || name == "pageSize")
			{
				bool& assigned = name == "pageIndex" ? pageIndexSet : pageSizeSet;
				uint64_t parsed = 0;
				const auto parseResult = std::from_chars(
					value.data(), value.data() + value.size(), parsed);
				if (assigned || value.empty() || parseResult.ec != std::errc() ||
					parseResult.ptr != value.data() + value.size())
				{
					valid = false;
					break;
				}
				assigned = true;
				if (name == "pageIndex")
					query->pageIndex = parsed;
				else
					query->pageSize = parsed;
			}
			else if (name == "keyword")
			{
				query->keyword = URIUtil::urlDecode(value.c_str());
			}
			else if (name == "categoryCode")
			{
				query->categoryCode = URIUtil::urlDecode(value.c_str());
			}
		}
		if (!valid || !pageIndexSet || !pageSizeSet || !query->validate().empty())
		{
			return createDtoResponse(Status::CODE_400,
				makeError("treatment.message.params-invalid", 10001));
		}

		// 调用执行函数并将业务码映射为 HTTP 状态。
		auto result = execQueryOptionsV2(query, v2AuthObject->getPayload());
		const int code = result->code.getValue(10099);
		if (code == 10003)
			return createDtoResponse(Status::CODE_403, result);
		if (code == 10099)
			return createDtoResponse(Status::CODE_500, result);
		return createDtoResponse(Status::CODE_200, result);
	}

	// 获取诊疗列表（条件+分页）：接口信息
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("treatment.query.summary"), queryTreatment,
		TreatmentQuery, TreatmentPageJsonVO::Wrapper, API_TAG);
	// 获取诊疗列表（条件+分页）：接口绑定
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/treatment/query", queryTreatment,
		TreatmentQuery, execQueryTreatment(query));



	//// 导入诊疗：接口信息
	//API_DEF_ENDPOINT_INFO_FILE_AUTH(ZH_WORDS_GETTER("treatment.import.summary"), importTreatment, TreatmentImportDTO::Wrapper, StringJsonVO::Wrapper, API_TAG);
	//// 导入诊疗：接口绑定
	//API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/treatment/import", importTreatment, REQUEST(std::shared_ptr<IncomingRequest>, request), execImportTreatment(request, authObject->getPayload()));

	//// 3.1 定义测试声明式服务调用的接口1描述
	//API_DEF_ENDPOINT_INFO_AUTH(
	//	ZH_WORDS_GETTER("treatment.query-one.summary"), queryOne, TreatmentDetailJsonVO::Wrapper, API_TAG2,
	//	API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("treatment.field.id"), "184000000000000001", true););
	//// 3.2 定义测试声明式服务调用的接口1处理
	//API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/treatment/query-one", queryOne, QUERY(String, id), execQueryOne(id, authObject->getPayload()));

	//// 3.1 定义测试声明式服务调用的接口2描述
	//API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("treatment.query-all.summary"), queryAll, TreatmentQuery, TreatmentPageJsonVO::Wrapper, API_TAG2);
	//// 3.2 定义测试声明式服务调用的接口2处理
	//API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/treatment/query-all", queryAll, TreatmentQuery, execQueryAll(query, authObject->getPayload()));

	//// 3.1 定义导出接口描述Excel
	//API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("treatment.export.summary"), exportTreatment, TreatmentQuery, Void, API_TAG);
	//// 3.2 定义导出接口处理
	//API_HANDLER_ENDPOINT_OPTION_AUTH(API_M_GET, "/treatment/export", exportTreatment, QUERIES(QueryParams, queryParams),
	//	API_HANDLER_QUERY_PARAM(query, TreatmentQuery, queryParams); return execExportTreatment(query););

	//// 3.1 定义导出接口描述pdf
	//API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("treatment.export-pdf.summary"), exportTreatmentPdf, TreatmentQuery, StringJsonVO::Wrapper, API_TAG);
	//// 3.2 定义导出接口处理
	//API_HANDLER_ENDPOINT_OPTION_AUTH(API_M_GET, "/treatment/export-pdf", exportTreatmentPdf, QUERIES(QueryParams, queryParams),
	//	API_HANDLER_QUERY_PARAM(query, TreatmentQuery, queryParams); API_HANDLER_RESP_VO(execExportTreatmentPdf(query)););

private: // 接口执行函数
	// 通过ID获取诊疗详情
	TreatmentDetailJsonVO::Wrapper execQueryById(const String& id);
	
	
	// 新增诊疗：执行函数
	StringJsonVO::Wrapper execAddTreatment(const TreatmentAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 修改诊疗：执行函数
	StringJsonVO::Wrapper execModifyTreatment(const TreatmentModifyDTO::Wrapper& dto, const PayloadDTO& payload);

	// 启/停用诊疗（支持批量）：执行函数
	StringJsonVO::Wrapper execModifyStatus(const TreatmentStatusDTO::Wrapper& dto, const PayloadDTO& payload);

	// 演示删除数据
	ListJsonVO<String>::Wrapper execRemoveTreatment(const List<String>& ids);

	// 获取选择诊疗列表执行函数。
	TreatmentOptionsPageJsonVO::Wrapper execQueryOptionsV2(
		const TreatmentOptionsQuery::Wrapper& query, const PayloadDTO& payload);

	// 获取诊疗列表（条件+分页）：执行函数
	TreatmentPageJsonVO::Wrapper execQueryTreatment(const TreatmentQuery::Wrapper& query);

	//// 声明式服务查询一条
	//TreatmentDetailJsonVO::Wrapper execQueryOne(const String& id, const PayloadDTO& payload);

	//// 测试声明式服务调用2
	//TreatmentPageJsonVO::Wrapper execQueryAll(const TreatmentQuery::Wrapper& query, const PayloadDTO& payload);

	//// 导入诊疗
	//StringJsonVO::Wrapper execImportTreatment(std::shared_ptr<IncomingRequest> request, const PayloadDTO& payload);
	
	//// 演示导出数据
	//std::shared_ptr<OutgoingResponse> execExportTreatment(const TreatmentQuery::Wrapper& query);
	//// 演示导出数据(PDF)
	//StringJsonVO::Wrapper execExportTreatmentPdf(const TreatmentQuery::Wrapper& query);
};

#undef API_TAG
#undef API_TAG2
#include OATPP_CODEGEN_END(ApiController)

#endif // !_TREATMENTCONTROLLER_H_
