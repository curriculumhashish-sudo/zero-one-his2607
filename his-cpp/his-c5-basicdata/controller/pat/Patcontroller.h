#pragma once
#ifndef _PAT_CONTROLLER_
#define _PAT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pat/PatPageQuery.h"
#include "domain/query/pat/PatSelectPageQuery.h"
#include "domain/vo/pat/PatPageJsonVO.h"
#include "domain/vo/pat/PatSelectPageJsonVO.h"
#include "ApiHelper.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("pat.tag")

/**
 * Patient management controller.
 */
class PatController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatController);

public:
	// Paginated patient list (placeholder for other teammate features)
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("pat.page.summary"), queryPatPage, PatPageQuery, PatPageJsonVO::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/pat/page", queryPatPage, PatPageQuery, execQueryPatPage(query, authObject->getPayload()));

	// Select-patient popup paginated list
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("pat.select.page.summary"), querySelectPatPage, PatSelectPageQuery, PatSelectPageJsonVO::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/pat/select/page", querySelectPatPage, PatSelectPageQuery, execQuerySelectPatPage(query, authObject->getPayload()));

	// Delete patients (placeholder)
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("pat.delete.summary"), removePat, ListJsonVO<String>::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/pat/delete", removePat, BODY_DTO(List<String>, ids), execRemovePat(ids));

private:
	PatPageJsonVO::Wrapper execQueryPatPage(const PatPageQuery::Wrapper& query, const PayloadDTO& payload);
	PatSelectPageJsonVO::Wrapper execQuerySelectPatPage(const PatSelectPageQuery::Wrapper& query, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemovePat(const List<String>& ids);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif // _PAT_CONTROLLER_
