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
#include "stdafx.h"
#include "TreatmentController.h"
#include "../../service/sample/TreatmentService.h"
//#include "../../domain/bo/PdfReportBO.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"

TreatmentPageJsonVO::Wrapper TreatmentController::execQueryTreatment(const TreatmentQuery::Wrapper& query)
{
	// 查询数据
	auto result = TreatmentService().listAll(query);
	// 响应结果
	auto jvo = TreatmentPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

TreatmentDetailJsonVO::Wrapper TreatmentController::execQueryOne(const String& id)
{
	// 定义返回数据对象
	auto vo = TreatmentDetailJsonVO::createShared();

	// 参数校验：详情主键不能为空
	if (!id || id->empty())
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}

	// Service/DAO 未接通前，校验通过后仍返回示例数据
	auto dto = buildSampleTreatment(id, "AD00000001", u8"\u4ee3\u714e\u4e2d\u836f");
	vo->success(dto);
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execModifyStatus(const TreatmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate the request and return a sample success response.
	auto vo = StringJsonVO::createShared();
	if (!dto || !dto->ids || dto->ids->size() == 0) {
		vo->fail("ids empty");
		return vo;
	}
	vo->success("OK");
	return vo;
}

ListJsonVO<String>::Wrapper TreatmentController::execRemoveTreatment(const List<String>& ids)
{
	// 定义返回数据对象
	auto jvo = ListJsonVO<String>::createShared();
	// 参数校验
	if (ids->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	// 执行数据删除
	if (SampleService().removeData(ids)) {
		jvo->success(ids);
	}
	else
	{
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

TreatmentSelectPageJsonVO::Wrapper TreatmentController::execQuerySelect(const TreatmentQuery::Wrapper& query)
{
	auto pageDto = TreatmentSelectPageDTO::createShared();

	auto first = TreatmentSelectDTO::createShared();
	first->id = "184000000000000001";
	first->name = u8"\u4ee3\u714e\u4e2d\u836f";
	pageDto->addData(first);

	auto second = TreatmentSelectDTO::createShared();
	second->id = "184000000000000002";
	second->name = u8"\u6ce8\u5c04\u7528\u836f";
	pageDto->addData(second);

	pageDto->pageIndex = query->pageIndex;
	pageDto->pageSize = query->pageSize;
	pageDto->total = 2;
	pageDto->calcPages();

	auto vo = TreatmentSelectPageJsonVO::createShared();
	vo->success(pageDto);
	return vo;
}

TreatmentPageJsonVO::Wrapper SampleController::execQueryAll(const TreatmentQuery::Wrapper& query, const PayloadDTO& payload)
{
	// 创建客户端对象
	API_CLIENT_CREATE(ac, om, TreatmentApiClient, "treatment-api");
	// 构建凭证
	std::string token = PayloadDTO::getTokenPrefix() + payload.getToken();
	// 返回查询结果
	return ac->queryAll(token, query->pageIndex, query->pageSize, URIUtil::urlEncode(query->name))->readBodyToDto<TreatmentPageJsonVO::Wrapper>(om);
}

StringJsonVO::Wrapper TreatmentController::execAddTreatment(
	const TreatmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto vo = StringJsonVO::createShared();

	// 参数校验
	if (!dto)
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		vo->init(errmsg, RS_PARAMS_INVALID);
		return vo;
	}

	// Service/DAO 未接通前，校验通过后仍返回示例主键
	vo->success("184000000000000002");
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execModifyTreatment(
	const TreatmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto vo = StringJsonVO::createShared();

	// 参数校验
	if (!dto)
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		vo->init(errmsg, RS_PARAMS_INVALID);
		return vo;
	}

	// Service/DAO 未接通前，校验通过后回传主键
	vo->success(dto->id);
	return vo;
}
