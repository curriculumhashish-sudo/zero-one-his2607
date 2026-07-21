#pragma once
#ifndef  _DRUGCONTROLLER_H_
#define _DRUGCONTROLLER_H_
#include "domain/vo/BaseJsonVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)
#include "../../GlobalInclude.h"

class DrugController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DrugController);
public:
	// 分页查询接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.export.summary"), exportSample, SampleQuery, Void, API_TAG);
private:
};
#include OATPP_CODEGEN_END(ApiController)

#endif 