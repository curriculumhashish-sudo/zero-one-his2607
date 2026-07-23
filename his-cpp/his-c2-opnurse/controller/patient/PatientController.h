#ifndef _PATIENTCONTROLLER_H_
#define _PATIENTCONTROLLER_H_

#include"domain/vo/BaseJsonVO.h"
#include"domain/query/patient/PatientQuery.h"
#include"domain/vo/patient/PatientVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("patient1.tags.tag")
class PatientController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatientController);
public:
	// Define paginated patient query endpoint info
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("patient1.query.summary"), queryPatient, PatientQuery, PatientPageJsonVO::Wrapper, API_TAG);
	// Define paginated patient query endpoint handler
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/patient/query", queryPatient, PatientQuery, executeQueryPatient(query));

	// Define paginated patient query executor
	PatientPageJsonVO::Wrapper executeQueryPatient(const PatientQuery::Wrapper& query);

};
#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif