#ifndef _PATIENTACTCONTROLLER_H_
#define _PATIENTACTCONTROLLER_H_

#include"domain/vo/patient/PatientVO.h"
#include"domain/query/patient/PatientAct.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("patient1.tags.tag")
class PatientActController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatientActController);
public:
	// Define patient act query endpoint info (non-paginated)
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("patient1.query.act"), actPatient, PatientActQuery, PatientActListJsonVO::Wrapper, API_TAG);
	// Define patient act query endpoint handler
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/patient/query-act", actPatient, PatientActQuery, executeActPatient(query));

	// Define patient act query executor
	PatientActListJsonVO::Wrapper executeActPatient(const PatientActQuery::Wrapper& query);

};
#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif