#pragma once
#ifndef _GET_CATEGORY_DETAILS_CONTROLLER_
#define _GET_CATEGORY_DETAILS_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/settle/GetCategoryDetailsQuery.h" 
#include "domain/vo/settle/GetCategoryDetailsVO.h"
#include "domain/dto/settle/GetCategoryDetailsDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("charging.settle.tag")

class GetCategoryDetailsController : public oatpp::web::server::api::ApiController {
public:
    API_ACCESS_DECLARE(GetCategoryDetailsController);

public:

    ENDPOINT_INFO(queryGetCategoryDetails) {
        API_DEF_ADD_TITLE(ZH_WORDS_GETTER("qi.charging.settle.summary3"));
        API_DEF_ADD_AUTH();

        API_DEF_ADD_RSP_JSON_WRAPPER(GetCategoryDetailsJsonVO);
        API_DEF_ADD_TAG(API_TAG);

        API_DEF_ADD_QUERY_PARAMS(String, "inpatient_no", ZH_WORDS_GETTER("qi.charging.settle.inpatient_no"), "", false);
        API_DEF_ADD_QUERY_PARAMS(String, "encounter_id", ZH_WORDS_GETTER("qi.charging.settle.encounter_id"), "", false);
    }


    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/settle/getCategoryDetails",
        queryGetCategoryDetails,
        GetCategoryDetailsQuery,
        execQueryGetCategoryDetails(query)
    );

private:
    GetCategoryDetailsJsonVO::Wrapper execQueryGetCategoryDetails(const GetCategoryDetailsQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif



