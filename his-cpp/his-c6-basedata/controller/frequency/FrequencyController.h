#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/21 00:00:00

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

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/frequency/FrequencyQuery.h"
#include "domain/vo/frequency/FrequencyVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("frequency.tags.tag")

class FrequencyController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(FrequencyController);
public:
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

private:
	ListJsonVO<FrequencyVO::Wrapper>::Wrapper executeGetFrequencyName(
		const FrequencyQuery::Wrapper& query);
	FrequencyPageJsonVO::Wrapper execQueryAll(const FrequencyQuery::Wrapper& query);
	FrequencyDetailJsonVO::Wrapper execQueryOne(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_FREQUENCYCONTROLLER_H_
