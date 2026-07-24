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
#include "DiagnosisController.h"
#include "../../service/Diagnosis/DiagnoseService.h"
#include <cctype>
#include <limits>

namespace
{
	std::string trimCopy(const std::string& value)
	{
		std::size_t begin = 0;
		while (begin < value.size()
			&& std::isspace(static_cast<unsigned char>(value[begin])) != 0)
		{
			++begin;
		}

		std::size_t end = value.size();
		while (end > begin
			&& std::isspace(static_cast<unsigned char>(value[end - 1])) != 0)
		{
			--end;
		}
		return value.substr(begin, end - begin);
	}
}

DiagnosisSelectPageJsonVO::Wrapper DiagnosisController::execQuerySelectDiagnosis(
	const DiagnosisSelectQuery::Wrapper& query)
{
	auto response = DiagnosisSelectPageJsonVO::createShared();
	if (!query || !query->pageIndex || !query->pageSize)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	const uint64_t pageIndex = query->pageIndex.getValue(0);
	const uint64_t pageSize = query->pageSize.getValue(0);
	if (pageIndex == 0 || pageSize == 0 || pageSize > 1000
		|| pageIndex > std::numeric_limits<uint64_t>::max() / pageSize)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	const std::string diagnosisKindCode = trimCopy(
		query->diagnosisKindCode.getValue(""));
	if (!diagnosisKindCode.empty()
		&& diagnosisKindCode != "1"
		&& diagnosisKindCode != "2")
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	query->diagnosisKindCode = diagnosisKindCode.empty()
		? oatpp::String(nullptr)
		: oatpp::String(diagnosisKindCode.c_str());

	const std::string keyword = trimCopy(query->keyword.getValue(""));
	query->keyword = keyword.empty()
		? oatpp::String(nullptr)
		: oatpp::String(keyword.c_str());

	response->success(DiagnoseService().listSelectableDiagnoses(query));
	return response;
}

DiagnosisDetailPageJsonVO::Wrapper DiagnosisController::execQueryAllDiagnosis(
	const DiagnosisPageQuery::Wrapper& query)
{
	auto page = DiagnosisDetailPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	page->total = 1;
	page->addData(DiagnosisDetailDTO::createShared());
	page->calcPages();

	auto response = DiagnosisDetailPageJsonVO::createShared();
	response->success(page);
	return response;
}

DiagnosisDetailJsonVO::Wrapper DiagnosisController::execQueryOneDiagnosis(
	const DiagnosisDetailQuery::Wrapper& query)
{
	auto response = DiagnosisDetailJsonVO::createShared();
	if (!query->id || query->id.getValue(0) <= 0)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	auto detail = DiagnosisDetailDTO::createShared();
	detail->id = query->id;
	response->success(detail);
	return response;
}

StringJsonVO::Wrapper DiagnosisController::execAddDiagnose(
	const DiagnoseAddDTO::Wrapper& dto,
	const PayloadDTO& payload)
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

StringJsonVO::Wrapper DiagnosisController::execModifyDiagnose(
	const DiagnoseDTO::Wrapper& dto,
	const PayloadDTO& payload)
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

ListJsonVO<String>::Wrapper DiagnosisController::execRemoveDiagnose(const List<String>& ids)
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

StringJsonVO::Wrapper DiagnosisController::execBatchChangeDiagnoseStatus(
	const DiagnoseBatchStatusDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto response = StringJsonVO::createShared();
	if (!dto->ids || !dto->status)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	if (*dto->status != 0 && *dto->status != 1)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	dto->setPayload(&payload);
	if (DiagnoseService().batchChangeStatus(dto->ids, dto->status))
	{
		response->success(nullptr);
	}
	else
	{
		response->fail(nullptr);
	}
	return response;
}
