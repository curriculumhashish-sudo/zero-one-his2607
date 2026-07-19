#pragma once
#ifndef _CHARGE_CONTROLLER_
#define _CHARGE_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include"domain/dto/charge/GetPatientQueueDTO.h"
#include"domain/vo/charge/GetPatientQueueJsonVO.h"
#include"domain/query/charge/GetPatientQueueQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("lhfc.charging.charge.tags.t1")
/*
	门诊收费控制器
*/
class ChargeController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(ChargeController);
public:
	//获取患者队列接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.summary"),
		queryPatientQueue,
		GetPatientQueueQuery,
		GetPatientQueueJsonVO::Wrapper, 
		API_TAG
	);
	//获取患者队列接口定义
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET, "/charge/query-patient-queue", queryPatientQueue,
		GetPatientQueueQuery, execQueryPatientQueue(query)
	);

private:
	GetPatientQueueJsonVO::Wrapper execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif //!_CHARGE_CONTROLLER_