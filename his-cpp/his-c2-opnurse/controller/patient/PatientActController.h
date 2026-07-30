#ifndef _PATIENTACTCONTROLLER_H_
#define _PATIENTACTCONTROLLER_H_

#include"domain/vo/patient/PatientVO.h"
#include"domain/query/patient/PatientAct.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("patient1.tags.tag")

class PatientActController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatientActController);
public:
	// Endpoint info definition
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("patient1.query.act"),
		actPatient, PatientActQuery, PatientActListJsonVO::Wrapper, API_TAG);
	// Endpoint binding
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/patient/query-act",
		actPatient, PatientActQuery, execActPatient(query));

private:
	// Execute patient act query
	PatientActListJsonVO::Wrapper execActPatient(const PatientActQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PATIENTACTCONTROLLER_H_