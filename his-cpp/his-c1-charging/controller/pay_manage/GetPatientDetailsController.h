#pragma once
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/GetPatientDetailsQuery.h"
#include "domain/dto/pay_manage/GetPatientDetailsDTO.h"
#include "domain/vo/pay_manage/GetPatientDetailsVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("byj.charging.pay_manage.tag2")

class GetPatientDetailsController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(GetPatientDetailsController);
public:
    
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("byj.charging.pay_manage.summary2"),
        getPatientDetails,
        GetPatientDetailsQuery,
        GetPatientDetailsVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charging/pay_manage/getpatientdetails", getPatientDetails,
        GetPatientDetailsQuery, execGetPatientDetails(query)
    );

private:
    GetPatientDetailsVO::Wrapper execGetPatientDetails(const GetPatientDetailsQuery::Wrapper& query);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)