/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/22 10:00:00

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
#include "DiagnoseController.h"
#include "../../service/Diagnose/DiagnoseService.h"
//#include "DiagnoseController.h"
//#include "../../service/Diagnose/DiagnoseService.h"
#include <algorithm>
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b:his-cpp/his-c6-basedata/controller/Diagnosis/DiagnosisController.cpp
#include <cctype>
#include <exception>

bool DiagnoseController::hasValidQueryParameters(const QueryParams& queryParams)
{
	for (const auto& parameter : queryParams.getAll())
	{
		const std::string name = parameter.first.toString();
		if (name != "keyword" && name != "diagnoseKindCode"
			&& name != "pageIndex" && name != "pageSize")
		{
			return false;
		}
	}

//你修改一下controller层的接口吧，这里的接口和controller.h层的接口根本对不上，每个函数都爆红，改不了
//bool DiagnosisController::hasValidQueryParameters(const QueryParams& queryParams)
//{
//	for (const auto& parameter : queryParams.getAll())
//	{
//		const std::string name = parameter.first.toString();
//		if (name != "keyword" && name != "diagnosisKindCode"
//			&& name != "pageIndex" && name != "pageSize")
//		{
//			return false;
//		}
//	}
//
//	const auto isUnsignedInteger = [&queryParams](const char* name)
//	{
//		const auto value = queryParams.get(name);
//		if (!value)
//		{
//			return false;
//		}
//		const std::string text = value.getValue("");
//		return !text.empty()
//			&& std::all_of(
//				text.begin(),
//				text.end(),
//				[](unsigned char character)
//				{
//					return std::isdigit(character) != 0;
//				});
//	};
//
//	return isUnsignedInteger("pageIndex") && isUnsignedInteger("pageSize");
//}
///*
////DiagnoseSelectPageJsonVO::Wrapper DiagnoseController::execQuerySelectDiagnose(
////	const DiagnoseSelectQuery::Wrapper& query)
////{
//	auto response = DiagnoseSelectPageJsonVO::createShared();
//	if (!query || !query->pageIndex || !query->pageSize)
//*/
//		DiagnoseSelectPageJsonVO::Wrapper DiagnoseController::execQuerySelectDiagnose(
//				const DiagnoseSelectQuery::Wrapper& query,
//	const PayloadDTO& payload)
//{
//	// 定义返回数据对象。
//	auto response = DiagnoseSelectPageJsonVO::createShared();
//	// 参数校验。
//	if (!query || !query->validate().empty() || payload.getId().empty())
//	{
//		response->init(nullptr, RS_PARAMS_INVALID);
//		return response;
//	}
//
//	// 执行分页查询。
//	auto result = DiagnoseService().listSelectableDiagnose(query, payload);
//	if (!result)
//	{
//		response->init(nullptr, RS_FORBIDDEN);
//		return response;
//	}
//
//	const std::string diagnoseKindCode = trimCopy(
//		query->diagnoseKindCode.getValue(""));
//	if (!diagnoseKindCode.empty()
//		&& diagnoseKindCode != "1"
//		&& diagnoseKindCode != "2")
//	{
//		response->init(nullptr, RS_PARAMS_INVALID);
//		return response;
//	}
//	query->diagnoseKindCode = diagnoseKindCode.empty()
//		? oatpp::String(nullptr)
//		: oatpp::String(diagnoseKindCode.c_str());
//
//	const std::string keyword = trimCopy(query->keyword.getValue(""));
//	query->keyword = keyword.empty()
//		? oatpp::String(nullptr)
//		: oatpp::String(keyword.c_str());
//
//	response->success(DiagnoseService().listSelectableDiagnoses(query));
//	// 响应结果。
//	response->success(result);
//	return response;
//}
//
//DiagnoseDetailPageJsonVO::Wrapper DiagnoseController::execQueryAllDiagnose(
//	const DiagnosePageQuery::Wrapper& query)
//{
//	auto page = DiagnoseDetailPageDTO::createShared();
//	page->pageIndex = query->pageIndex;
//	page->pageSize = query->pageSize;
//	page->total = 1;
//	page->addData(DiagnoseDetailDTO::createShared());
//	page->calcPages();
//
//	auto response = DiagnoseDetailPageJsonVO::createShared();
//	response->success(page);
//	return response;
//}
//
//DiagnoseDetailJsonVO::Wrapper DiagnoseController::execQueryOneDiagnose(
//	const DiagnoseDetailQuery::Wrapper& query)
//{
//	auto response = DiagnoseDetailJsonVO::createShared();
//	if (!query->id || query->id.getValue(0) <= 0)
//	{
//		response->init(nullptr, RS_PARAMS_INVALID);
//		return response;
//	}
//
//	auto detail = DiagnoseDetailDTO::createShared();
//	detail->id = query->id;
//	response->success(detail);
//	return response;
//}

StringJsonVO::Wrapper DiagnoseController::execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = StringJsonVO::createShared();

	const std::string errorMessage = dto->validate();
	if (!errorMessage.empty())
	{
		response->init(errorMessage, RS_PARAMS_INVALID);
		return response;
	}

	dto->setPayload(&payload);
	const std::string id = DiagnoseService().saveData(dto);
	if (!id.empty())
	{
		response->success(id);
	}
	else
	{
		response->fail("");
	}
	return response;
}

StringJsonVO::Wrapper DiagnoseController::execModifyDiagnose(const DiagnoseDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = StringJsonVO::createShared();
	if (!dto->id)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	dto->setPayload(&payload);
	if (DiagnoseService().updateData(dto))
	{
		response->success(dto->id);
	}
	else
	{
		response->fail(dto->id);
	}
	return response;
}

ListJsonVO<String>::Wrapper DiagnoseController::execRemoveDiagnose(const List<String>& ids)
{
	auto response = ListJsonVO<String>::createShared();
	if (!ids || ids->empty())
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	if (DiagnoseService().removeData(ids))
	{
		response->success(ids);
	}
	else
	{
		response->fail(nullptr);
	}
	return response;
}

StringJsonVO::Wrapper DiagnoseController::execBatchChangeDiagnoseStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = StringJsonVO::createShared();
	if (!dto->ids || !dto->status)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	if (*dto->status != "STOP" && *dto->status != "ACTIVE")
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	dto->setPayload(&payload);
	if (DiagnoseService().batchChangeStatus(dto, dto->status))
	{
		response->success(nullptr);
	}
	else
	{
		response->fail(nullptr);
	}
	return response;
}
//逻辑删除（记录修改人）
DiagnoseRemoveJsonVO::Wrapper DiagnoseController::execRemoveDiagnosis(const DiagnoseRemoveDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = DiagnoseRemoveJsonVO::createShared();
	if (!dto->ids)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	dto->setPayload(&payload);
	if (DiagnoseService().execRemoveData(dto))
	{
		response->success(dto);
	}
	else
	{
		response->fail(nullptr);
	}
	return response;
}
