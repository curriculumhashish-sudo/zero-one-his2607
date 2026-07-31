/*
 Copyright Zero One Star. All rights reserved.


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
#include "FrequencyController.h"
#include "../../service/frequency/FrequencyService.h"
#include "../../domain/bo/PdfReportBO.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"
#include<algorithm>
// 获取频次名称
ListJsonVO<FrequencyVO::Wrapper>::Wrapper FrequencyController::execGetFrequencyName(
	const FrequencyQuery::Wrapper& query)
{
	auto result = FrequencyService().getName(query);
	auto jvo = ListJsonVO<FrequencyVO::Wrapper>::createShared();

	jvo->success(result);
	return jvo;
}

//获取频次列表
FrequencyPageJsonVO::Wrapper FrequencyController::execQueryFrequency(
	const FrequencyQuery::Wrapper& query)
{
	auto result = FrequencyService().listAll(query);
	auto jvo = FrequencyPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

//获取频次详情
FrequencyDetailJsonVO::Wrapper FrequencyController::execQueryById(const String& id)
{
	auto jvo = FrequencyDetailJsonVO::createShared();
	if (!id) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	auto res = FrequencyService().getById(id.getValue({}));
	jvo->success(res);
	return jvo;
}

// ==================== 2. 增删改接口（yancao 增补实现） ====================

StringJsonVO::Wrapper FrequencyController::execAdd(const FrequencyDTO::Wrapper& dto)
{
	auto jvo = StringJsonVO::createShared();

	std::string errorMsg;
	if (FrequencyService().addFrequency(dto, errorMsg))
	{
		jvo->success(dto->id ? dto->id.getValue({}) : "");
	}
	else
	{
		jvo->init(errorMsg, RS_PARAMS_INVALID);
	}
	return jvo;
}

BooleanJsonVO::Wrapper FrequencyController::execModify(const FrequencyDTO::Wrapper& dto)
{
	auto jvo = BooleanJsonVO::createShared();

	if (!dto->id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	std::string errorMsg;
	if (FrequencyService().modifyFrequency(dto, errorMsg))
	{
		jvo->success(true);
	}
	else
	{
		jvo->init(false, RS_PARAMS_INVALID);
	}
	return jvo;
}

BooleanJsonVO::Wrapper FrequencyController::execModifyStatus(const String& id, const Int32& status)
{
	auto jvo = BooleanJsonVO::createShared();

	// 参数校验
	if (!id || !status)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行启/停用
	std::string errorMsg;
	if (FrequencyService().modifyStatus(id.getValue({}), status.getValue(0), errorMsg))
	{
		jvo->success(true);
	}
	else
	{
		jvo->init(false, RS_PARAMS_INVALID);
	}

	// 响应结果
	return jvo;
}

BooleanJsonVO::Wrapper FrequencyController::execDelete(const String& id)
{
	auto jvo = BooleanJsonVO::createShared();

	if (!id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	std::string errorMsg;
	if (FrequencyService().removeFrequency(id.getValue({}), errorMsg))
	{
		jvo->success(true);
	}
	else
	{
		jvo->init(false, RS_PARAMS_INVALID);
	}
	return jvo;
}