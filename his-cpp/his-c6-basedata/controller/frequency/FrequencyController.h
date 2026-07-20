#pragma once
#ifndef _FREQUENCYCONTROLLER_H_
#define _FREQUENCYCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/frequency/FrequencyQuery.h"
#include "domain/vo/frequency/FrequencySimpleVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("frequency.tags.tag")

class FrequencyController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(FrequencyController);
public:
	// 获取频次名称列表（下拉）
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("frequency.getName.summary"), getFrequencyName, FrequencyQuery, Vector<FrequencySimpleVO::Wrapper>, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/frequency/getName", getFrequencyName, FrequencyQuery, executeGetFrequencyName(query));

private:
	Vector<FrequencySimpleVO::Wrapper> executeGetFrequencyName(const FrequencyQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_FREQUENCYCONTROLLER_H_