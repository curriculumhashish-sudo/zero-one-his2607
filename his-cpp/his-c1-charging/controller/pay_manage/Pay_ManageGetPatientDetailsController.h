#pragma once
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientDetailsQuery.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientDetailsDTO.h"
#include "domain/vo/pay_manage/Pay_ManageGetPatientDetailsVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.tag")

class Pay_ManageGetPatientDetailsController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(Pay_ManageGetPatientDetailsController);
public:
    
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("charging.pay_manage.GetPatientDetails.summary"),
        Pay_ManageGetPatientDetails,
        Pay_ManageGetPatientDetailsQuery,
        Pay_ManageGetPatientDetailsVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charging/pay_manage/getpatientdetails", Pay_ManageGetPatientDetails,
        Pay_ManageGetPatientDetailsQuery, execPay_ManageGetPatientDetails(query)
    );

private:
    Pay_ManageGetPatientDetailsVO::Wrapper execPay_ManageGetPatientDetails(const Pay_ManageGetPatientDetailsQuery::Wrapper& query);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)