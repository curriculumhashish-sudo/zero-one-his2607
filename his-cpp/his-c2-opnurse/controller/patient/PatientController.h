#ifndef _PATIENTCONTROLLER_H_
#define _PATIENTCONTROLLER_H_

#include"domain/vo/BaseJsonVO.h"
#include"domain/query/patient/PatientQuery.h"
#include"domain/vo/patient/PatientVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("patient.tags.tag")
class PatientController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatientController);
public:
	// 定义分页患者查询端点信息
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("patient.query.summary"), queryPatient, PatientQuery, PatientActPageJsonVO::Wrapper, API_TAG);
	// 定义分页患者查询端点处理器
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/patient", queryPatient, PatientQuery, executeQueryPatient(query));

private:
	// 定义分页患者查询执行器
	PatientActPageJsonVO::Wrapper executeQueryPatient(const PatientQuery::Wrapper& query);

};
#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif