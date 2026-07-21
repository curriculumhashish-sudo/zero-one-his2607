#pragma once
#ifndef _PATIENTLEAVECONTROLLER_H_
#define _PATIENTLEAVECONTROLLER_H_

#include "domain/dto/cycleCtrl/patientLeaveDTO.h"
#include "domain/query/cycleCtrl/patientLeaveQuery.h"
#include "domain/vo/cycleCtrl/patientLeaveVo.h"
#include "domain/GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("title.tags.text")

class PatientLeaveController : public oatpp::web::server::api::ApiController
{

    API_ACCESS_DECLARE(PatientLeaveController);
public:
    ENDPOINT(API_M_PUT, "/cycleCtrl/leave", leavePatient,BODY_DTO(Object<PatientLeaveQuery>, leaveParam))
    {
        API_HANDLER_RESP_VO(execLeavePatient(leaveParam));
    }

    ENDPOINT_INFO(leavePatient)
    {
        API_DEF_ADD_TITLE(ZH_WORDS_GETTER("leave.get.summary"));
        //∂®“Â±Í«©
        API_DEF_ADD_TAG(API_TAG);


        API_DEF_ADD_RSP_JSON_WRAPPER(PatientLeaveJsonVO);
    }

private:
    PatientLeaveJsonVO::Wrapper execLeavePatient(const PatientLeaveQuery::Wrapper& query);
};




#include OATPP_CODEGEN_END(ApiController)

#endif // !_PATIENTLEAVECONTROLLER_H_
