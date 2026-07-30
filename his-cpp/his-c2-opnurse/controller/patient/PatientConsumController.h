#ifndef _PATIENTCONSUMCONTROLLER_H_
#define _PATIENTCONSUMCONTROLLER_H_

#include"domain/vo/patient/PatientVO.h"
#include"domain/query/patient/PatientConsumable.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("patient1.tags.tag")

class PatientConsumController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatientConsumController);
public:
	// Endpoint info definition
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("patient1.queryconsum.consum"),
		consumPatient, PatientConsumQuery, PatientConsumListJsonVO::Wrapper, API_TAG);
	// Endpoint binding
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/patient/query-consum",
		consumPatient, PatientConsumQuery, execConsumPatient(query));

private:
	// Execute patient consumable query
	PatientConsumListJsonVO::Wrapper execConsumPatient(const PatientConsumQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PATIENTCONSUMCONTROLLER_H_