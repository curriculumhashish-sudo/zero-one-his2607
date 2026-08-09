#pragma once
#ifndef PAGEDIAGNOSISDICTIONARYCONTROLLER_H
#define PAGEDIAGNOSISDICTIONARYCONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/PayloadDTO.h"
#include "domain/GlobalInclude.h"
#include "domain/query/pageDiagnosisDictionary/PageDiagnosisDictionaryQuery.h"
#include "domain/vo/pageDiagnosisDictionary/PageDiagnosisDictionaryVo.h"



#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("dict.tags.text")

class PageDiagnosisDictionaryController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PageDiagnosisDictionaryController);

public:
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("dict.controller.summary"), 
		queryPageDict, PageDiagnosisDictionaryQuery, PageDiagnosisDictionaryPageVo::Wrapper, API_TAG);

	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/doctor-station/page-diagnosis-dictionary", 
		queryPageDict, PageDiagnosisDictionaryQuery, executeQueryPageDict(query,authObject->getPayload()));

private:
	PageDiagnosisDictionaryPageVo::Wrapper executeQueryPageDict(const PageDiagnosisDictionaryQuery::Wrapper& query, const PayloadDTO& payload);

};


#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !



