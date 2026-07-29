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

// 1. select
DiagnoseSelectJsonVO::Wrapper DiagnoseController::executeListSelectDiagnose(const DiagnoseSelectQuery::Wrapper& query)
{
	DiagnoseService ds;
	auto vo = DiagnoseSelectJsonVO::createShared();
	auto pageData = ds.listDiagnoseSelect(query);
	if (pageData)
		vo->success(pageData);
	else
		vo->fail({});
	return vo;
}
// 2.page
DiagnosePageJsonVO::Wrapper DiagnoseController::executeListPageDiagnose(const DiagnosePageQuery::Wrapper& query)
{
	DiagnoseService ds;
	auto vo = DiagnosePageJsonVO::createShared();
	vo->success(ds.listDiagnosePage(query));
	
	return vo;
}
// 3. detail
DiagnoseDetailJsonVO::Wrapper DiagnoseController::executeGetDiagnoseDetail(const String& id)
{
	DiagnoseService ds;
	auto jvo = DiagnoseDetailJsonVO::createShared();
	auto data = ds.getDiagnoseDetail(id);
	if (data)
		jvo->success(data);
	else
		jvo->fail({});
	return jvo;
}
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
//物理批量删除
ListJsonVO<String>::Wrapper DiagnoseController::execRemoveDiagnose(const List<String>& ids)
{
	auto response = ListJsonVO<String>::createShared();
	if (!ids || ids->empty())
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	if (DiagnoseService().RemoveData(ids))
	{
		response->success(ids);
	}
	else
	{
		response->fail(nullptr);
	}
	return response;
}

//逻辑批量删除不记录删除人
ListJsonVO<String>::Wrapper DiagnoseController::execremoveDiagnose(const List<String>& ids)
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
