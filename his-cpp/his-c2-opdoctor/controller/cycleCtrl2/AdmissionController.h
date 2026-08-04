#pragma once

#ifndef _ADMISSIONCONTROLLER_H_
#define _ADMISSIONCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include"domain/query/cycleCtrl2-admission/AdmissionQuery.h"
#include"../../domain/vo/doctor/Admission2VO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG2 ZH_WORDS_GETTER("cyclectrl2.test.tag")

class Admission2Controller : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(Admission2Controller);
public:
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("cyclectrl2.admission.apply"), apply2Admission, Admission2Query, Admission2ResultJsonVO::Wrapper, API_TAG2);
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_POST, "/cyclectrl/admission_apply", apply2Admission, Admission2Query, executeApplyAdmission(query, authObject->getPayload()));
private:
    Admission2ResultJsonVO::Wrapper executeApplyAdmission(const Admission2Query::Wrapper& query, const PayloadDTO& payload);
};
#undef API_TAG2

#include OATPP_CODEGEN_END(ApiController)
#endif
