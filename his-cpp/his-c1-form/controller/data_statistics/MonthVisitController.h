#ifndef _MONTH_VISIT_CONTROLLER_
#define _MONTH_VISIT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/data_statistics/MonthVisitQuery.h"
#include "domain/vo/data_statistics/MonthVisitVO.h"


#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("form.data_statistics.tag")


class MonthVisitController : public oatpp::web::server::api::ApiController
{

	
	API_ACCESS_DECLARE(MonthVisitController);


public:

	
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("Lin.form.data_statistics.query_month_visit.summary"),
		queryMonthVisit,
		MonthVisitQuery,
		MonthVisitPageJsonVO::Wrapper,
		API_TAG
	);



	
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/data-statistics/month-visit",
		queryMonthVisit,
		MonthVisitQuery,
		execQueryMonthVisit(query, authObject->getPayload())
	);



private:

	
	MonthVisitPageJsonVO::Wrapper execQueryMonthVisit(
		const MonthVisitQuery::Wrapper& query,
		const PayloadDTO& payload
	);

};


#undef API_TAG


#include OATPP_CODEGEN_END(ApiController)

#endif // !_MONTH_VISIT_CONTROLLER_