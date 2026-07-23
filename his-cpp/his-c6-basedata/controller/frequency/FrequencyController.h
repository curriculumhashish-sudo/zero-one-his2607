#pragma once
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
#ifndef _FREQUENCYCONTROLLER_H_
#define _FREQUENCYCONTROLLER_H_

#include "../ApiDeclarativeServicesHelper.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/frequency/FrequencyQuery.h"
#include "domain/vo/frequency/FrequencyVO.h"
#include "domain/dto/frequency/FrequencyDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#ifdef API_TAG
#undef API_TAG
#endif
#define API_TAG ZH_WORDS_GETTER("frequency.tags.tag")

class FrequencyController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(FrequencyController);
public:
	// ==================== 1. 查询类接口（完整保留队友 senma 的定义） ====================

	// Query enabled frequency names for selection controls.
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("frequency.getname.summary"), getFrequencyName,
		FrequencyQuery, ListJsonVO<FrequencyVO::Wrapper>::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET, "/frequency/getname", getFrequencyName,
		FrequencyQuery, executeGetFrequencyName(query));

	// Query the frequency list with filters and pagination.
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("frequency.query-all.summary"), queryAllFrequency,
		FrequencyQuery, FrequencyPageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET, "/frequency/query-all", queryAllFrequency,
		FrequencyQuery, execQueryAll(query));

	// Query one frequency by its identifier.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("frequency.query-one.summary"), queryOneFrequency,
		FrequencyDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(
			String, "id", ZH_WORDS_GETTER("frequency.field.id"), "1", true););
	API_HANDLER_ENDPOINT_AUTH(
		API_M_GET, "/frequency/query-one", queryOneFrequency,
		QUERY(String, id), execQueryOne(id));

	// ==================== 2. 增删改接口（yancao 实现：采用 Oat++ 原生宏） ====================

	// 新增频次
	ENDPOINT_INFO(addFrequency) {
		info->summary = ZH_WORDS_GETTER("frequency.add.summary");
		info->addTag(API_TAG);
	}
	ENDPOINT(API_M_POST, "/frequency/add", addFrequency,
		BODY_DTO(FrequencyDTO::Wrapper, dto))
	{
		return createDtoResponse(Status::CODE_200, execAdd(dto));
	}

	// 修改频次
	ENDPOINT_INFO(modifyFrequency) {
		info->summary = ZH_WORDS_GETTER("frequency.modify.summary");
		info->addTag(API_TAG);
	}
	ENDPOINT(API_M_PUT, "/frequency/modify", modifyFrequency,
		BODY_DTO(FrequencyDTO::Wrapper, dto))
	{
		return createDtoResponse(Status::CODE_200, execModify(dto));
	}

	// 修改频次状态
	ENDPOINT_INFO(modifyFrequencyStatus) {
		info->summary = ZH_WORDS_GETTER("frequency.modify-status.summary");
		info->addTag(API_TAG);
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("frequency.field.id"), "1", true);
		API_DEF_ADD_QUERY_PARAMS(Int32, "status", ZH_WORDS_GETTER("frequency.field.status"), 1, true);
	}
	ENDPOINT(API_M_PUT, "/frequency/modify-status", modifyFrequencyStatus,
		QUERY(String, id),
		QUERY(Int32, status))
	{
		return createDtoResponse(Status::CODE_200, execModifyStatus(id, status));
	}

	// 删除频次
	ENDPOINT_INFO(removeFrequency) {
		info->summary = ZH_WORDS_GETTER("frequency.remove.summary");
		info->addTag(API_TAG);
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("frequency.field.id"), "1", true);
	}
	ENDPOINT(API_M_DEL, "/frequency/remove", removeFrequency,
		QUERY(String, id))
	{
		return createDtoResponse(Status::CODE_200, execDelete(id));
	}

private:
	// 查询接口处理函数（senma）
	ListJsonVO<FrequencyVO::Wrapper>::Wrapper executeGetFrequencyName(
		const FrequencyQuery::Wrapper& query);
	FrequencyPageJsonVO::Wrapper execQueryAll(const FrequencyQuery::Wrapper& query);
	FrequencyDetailJsonVO::Wrapper execQueryOne(const String& id);

	// 增删改接口处理函数（yancao）
	StringJsonVO::Wrapper execAdd(const FrequencyDTO::Wrapper& dto);
	BooleanJsonVO::Wrapper execModify(const FrequencyDTO::Wrapper& dto);
	BooleanJsonVO::Wrapper execModifyStatus(const String& id, const Int32& status);
	BooleanJsonVO::Wrapper execDelete(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_FREQUENCYCONTROLLER_H__