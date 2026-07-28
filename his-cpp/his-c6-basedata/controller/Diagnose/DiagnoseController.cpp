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

DiagnoseSelectPageJsonVO::Wrapper DiagnoseController::execQuerySelectDiagnose(
	const DiagnoseSelectQuery::Wrapper& query)
{
	auto response = DiagnoseSelectPageJsonVO::createShared();
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

	const std::string diagnoseKindCode = trimCopy(
		query->diagnoseKindCode.getValue(""));
	if (!diagnoseKindCode.empty()
		&& diagnoseKindCode != "1"
		&& diagnoseKindCode != "2")
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	query->diagnoseKindCode = diagnoseKindCode.empty()
		? oatpp::String(nullptr)
		: oatpp::String(diagnoseKindCode.c_str());

	const std::string keyword = trimCopy(query->keyword.getValue(""));
	query->keyword = keyword.empty()
		? oatpp::String(nullptr)
		: oatpp::String(keyword.c_str());

	response->success(DiagnoseService().listSelectableDiagnoses(query));
	return response;
}

DiagnoseDetailPageJsonVO::Wrapper DiagnoseController::execQueryAllDiagnose(
	const DiagnosePageQuery::Wrapper& query)
{
	auto page = DiagnoseDetailPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	page->total = 1;
	page->addData(DiagnoseDetailDTO::createShared());
	page->calcPages();

	auto response = DiagnoseDetailPageJsonVO::createShared();
	response->success(page);
	return response;
}

DiagnoseDetailJsonVO::Wrapper DiagnoseController::execQueryOneDiagnose(
	const DiagnoseDetailQuery::Wrapper& query)
{
	auto response = DiagnoseDetailJsonVO::createShared();
	if (!query->id || query->id.getValue(0) <= 0)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	auto detail = DiagnoseDetailDTO::createShared();
	detail->id = query->id;
	response->success(detail);
	return response;
}

StringJsonVO::Wrapper DiagnoseController::execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto,const PayloadDTO& payload)
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

StringJsonVO::Wrapper DiagnoseController::execModifyDiagnose(const DiagnoseDTO::Wrapper& dto,const PayloadDTO& payload)
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

StringJsonVO::Wrapper DiagnoseController::execBatchChangeDiagnoseStatus(const DiagnoseBatchStatusDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto response = StringJsonVO::createShared();
	if (!dto->ids || !dto->status)
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}
	if (*dto->status!="STOP" && *dto->status != "ACTIVE")
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
//Âß¼­É¾³ý
DiagnoseRemoveJsonVO::Wrapper DiagnoseController::execRemoveDiagnose(const DiagnoseRemoveDTO::Wrapper& dto, const PayloadDTO& payload)
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