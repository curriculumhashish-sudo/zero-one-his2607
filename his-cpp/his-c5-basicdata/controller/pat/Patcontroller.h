#pragma once
#ifndef _PAT_CONTROLLER_
#define _PAT_CONTROLLER_

#include "ApiHelper.h"
#include "domain/dto/pat/PatientDTO.h"
#include "domain/query/pat/PatPageQuery.h"
#include "domain/query/pat/PatSelectPageQuery.h"
#include "domain/query/pat/PatientQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/pat/PatPageJsonVO.h"
#include "domain/vo/pat/PatSelectPageJsonVO.h"
#include "domain/vo/pat/PatientVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("pat.tag")

class PatController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PatController);

public:
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("pat.page.summary"), queryPatPage,
        PatPageQuery, PatPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/pat/page", queryPatPage, PatPageQuery,
        execQueryPatPage(query, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("pat.select.page.summary"), querySelectPatPage,
        PatSelectPageQuery, PatSelectPageJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/pat/select/page", querySelectPatPage, PatSelectPageQuery,
        execQuerySelectPatPage(query, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pat.query-by-id.summary"), queryPatientById,
        PatientDetailJsonVO::Wrapper, API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("pat.field.id"), "2233", true);
        info->addResponse<PatientDetailJsonVO::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<PatientDetailJsonVO::Wrapper>(Status::CODE_403, "application/json");
        info->addResponse<PatientDetailJsonVO::Wrapper>(Status::CODE_404, "application/json");
        info->addResponse<PatientDetailJsonVO::Wrapper>(Status::CODE_500, "application/json");
    );
    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_GET, "/pat/query-by-id", queryPatientById, QUERY(String, id),
        return execQueryById(id, authObject->getPayload());
    );

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pat.add.summary"), addPatient,
        StringJsonVO::Wrapper, API_TAG,
        info->addConsumes<PatientAddDTO::Wrapper>("application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_403, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_409, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_500, "application/json");
    );
    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_POST, "/pat/add", addPatient, BODY_DTO(PatientAddDTO::Wrapper, dto),
        return execAddPatient(dto, authObject->getPayload());
    );

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pat.modify.summary"), modifyPatient,
        StringJsonVO::Wrapper, API_TAG,
        info->addConsumes<PatientUpdateDTO::Wrapper>("application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_403, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_404, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_409, "application/json");
        info->addResponse<StringJsonVO::Wrapper>(Status::CODE_500, "application/json");
    );
    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_PUT, "/pat/modify", modifyPatient, BODY_DTO(PatientUpdateDTO::Wrapper, dto),
        return execModifyPatient(dto, authObject->getPayload());
    );

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pat.delete.summary"), removePat,
        ListJsonVO<String>::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL, "/pat/delete", removePat, BODY_DTO(List<String>, ids),
        execRemovePat(ids, authObject->getPayload()));

private:
    PatPageJsonVO::Wrapper execQueryPatPage(
        const PatPageQuery::Wrapper& query,
        const PayloadDTO& payload);
    PatSelectPageJsonVO::Wrapper execQuerySelectPatPage(
        const PatSelectPageQuery::Wrapper& query,
        const PayloadDTO& payload);
    std::shared_ptr<OutgoingResponse> execQueryById(
        const String& id,
        const PayloadDTO& payload);
    std::shared_ptr<OutgoingResponse> execAddPatient(
        const PatientAddDTO::Wrapper& dto,
        const PayloadDTO& payload);
    std::shared_ptr<OutgoingResponse> execModifyPatient(
        const PatientUpdateDTO::Wrapper& dto,
        const PayloadDTO& payload);
    ListJsonVO<String>::Wrapper execRemovePat(
        const List<String>& ids,
        const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _PAT_CONTROLLER_
