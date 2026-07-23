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

DiagnosisSelectPageJsonVO::Wrapper DiagnosisController::execQuerySelectDiagnosis(
	const DiagnosisSelectQuery::Wrapper& query)
{
	auto page = DiagnosisSelectPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	page->total = 1;
	page->addData(DiagnosisSelectDTO::createShared());
	page->calcPages();

	auto response = DiagnosisSelectPageJsonVO::createShared();
	response->success(page);
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
