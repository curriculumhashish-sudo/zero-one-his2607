#pragma once

#ifndef _GET_PATIENT_LIST_CONTROLLER_
#define _GET_PATIENT_LIST_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/settle/GetPatientListQuery.h"
#include "domain/vo/settle/GetPatientListVO.h"
#include "domain/DTO/settle/GetPatientListDTO.h"


#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("qi.charging.settle.tag1")


class GetPatientListController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(GetPatientListController);
public:
    ENDPOINT_INFO(queryGetPatientList) {       
        API_DEF_ADD_TITLE(ZH_WORDS_GETTER("qi.charging.settle.summary1"));
        API_DEF_ADD_AUTH();  
        API_DEF_ADD_RSP_JSON_WRAPPER(GetPatientListPageJsonVO);
        API_DEF_ADD_TAG(API_TAG);
        API_DEF_ADD_PAGE_PARAMS();
        API_DEF_ADD_QUERY_PARAMS(UInt64, "encounter_id", ZH_WORDS_GETTER("qi.charging.settle.encounter_id"), 1, false);
        API_DEF_ADD_QUERY_PARAMS(String, "inpatient_no", ZH_WORDS_GETTER("qi.charging.settle.inpatient_no"), "", false);
        API_DEF_ADD_QUERY_PARAMS(String, "patient_name", ZH_WORDS_GETTER("qi.charging.settle.patient_name"), "", false);
        API_DEF_ADD_QUERY_PARAMS(UInt8, "gender", ZH_WORDS_GETTER("qi.charging.settle.gender"), 1, false);
        API_DEF_ADD_QUERY_PARAMS(UInt32, "age", ZH_WORDS_GETTER("qi.charging.settle.age"), 18, false);
        API_DEF_ADD_QUERY_PARAMS(String, "dept_name", ZH_WORDS_GETTER("qi.charging.settle.dept_name"), "", false);
        API_DEF_ADD_QUERY_PARAMS(String, "ward_name", ZH_WORDS_GETTER("qi.charging.settle.ward_name"), "", false);
        API_DEF_ADD_QUERY_PARAMS(String, "bed_no", ZH_WORDS_GETTER("qi.charging.settle.bed_no"), "", false);
        API_DEF_ADD_QUERY_PARAMS(UInt64, "admit_start_date", ZH_WORDS_GETTER("qi.charging.settle.admit_start_date"), 1, true);
        API_DEF_ADD_QUERY_PARAMS(UInt64, "admit_end_date", ZH_WORDS_GETTER("qi.charging.settle.admit_end_date"), 25337073600000, true);
        API_DEF_ADD_QUERY_PARAMS(UInt8, "insurance_type", ZH_WORDS_GETTER("qi.charging.settle.insurance_type"), 1, false);
        API_DEF_ADD_QUERY_PARAMS(UInt8, "encounter_status", ZH_WORDS_GETTER("qi.charging.settle.encounter_status"), 1, false);

    }
    ENDPOINT(API_M_GET, "/charging/settle/getpatientlist", queryGetPatientList, QUERIES(QueryParams, queryParams), API_HANDLER_AUTH_PARAME) {
        API_HANDLER_QUERY_PARAM(getpatientlistQuery, GetPatientListQuery, queryParams);
        API_HANDLER_RESP_VO(execQueryGetPatientList(getpatientlistQuery));
    }
private:
    GetPatientListPageJsonVO::Wrapper execQueryGetPatientList(const GetPatientListQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif