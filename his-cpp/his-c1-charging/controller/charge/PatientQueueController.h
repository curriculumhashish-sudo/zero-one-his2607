#pragma once
#ifndef _PATIENT_QUEUE_CONTROLLER_
#define _PATIENT_QUEUE_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/charge/GetPatientQueueDTO.h"
#include "domain/vo/charge/GetPatientQueueJsonVO.h"
#include "domain/query/charge/GetPatientQueueQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("lhfc.charging.charge.tags.t1")
/*
   患者队列控制器
   负责分页+条件查询患者就诊队列
*/
class PatientQueueController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PatientQueueController);
public:
    // 获取患者队列（分页+条件查询）
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.GetPatientQueue.summary"),
        queryPatientQueue,
        GetPatientQueueQuery,
        GetPatientQueueJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charging/charge/get-patient-queue", queryPatientQueue,
        GetPatientQueueQuery, execQueryPatientQueue(query)
    );

private:
    GetPatientQueueJsonVO::Wrapper execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PATIENT_QUEUE_CONTROLLER_
