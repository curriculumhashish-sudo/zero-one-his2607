#pragma once
#ifndef _PATIENTFINISHCONTROLLER_H_
#define _PATIENTFINISHCONTROLLER_H_

#include "domain/dto/cycleCtrl/patientFinishDTO.h"
#include "domain/query/cycleCtrl/patientFinishQuery.h"
#include "domain/vo/cycleCtrl/patientFinishVo.h"
#include "domain/GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("title.tags.text")
class PatientFinishController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PatientFinishController);
public:
    // 完诊接口 PATCH
    ENDPOINT(API_M_PUT , "/cycleCtrl/finish", finishPatient, BODY_DTO(Object<PatientFinishQuery>, finishParam))
    {
        API_HANDLER_RESP_VO(execFinishPatient(finishParam));
    }

    ENDPOINT_INFO(finishPatient)
    {
        API_DEF_ADD_TITLE(ZH_WORDS_GETTER("finish.get.summary"));
        //定义标签
        API_DEF_ADD_TAG(API_TAG);
        API_DEF_ADD_RSP_JSON_WRAPPER(PatientFinishJsonVO);
    }

private:
    PatientFinishJsonVO::Wrapper execFinishPatient(const PatientFinishQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController)


#endif // !_PATIENTFINISHCONTROLLER_H_
