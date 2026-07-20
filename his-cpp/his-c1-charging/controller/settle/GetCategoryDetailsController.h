
#pragma once
#ifndef _GET_CATEGORY_DETAILS_CONTROLLER_
#define _GET_CATEGORY_DETAILS_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
// 确保路径正确
#include "domain/query/settle/GetCategoryDetailsQuery.h" 
#include "domain/vo/settle/GetCategoryDetailsVO.h"
#include "domain/dto/settle/GetCategoryDetailsDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("qi.charging.settle.tag3")

class GetCategoryDetailsController : public oatpp::web::server::api::ApiController {
public:
    API_ACCESS_DECLARE(GetCategoryDetailsController);

public:
    // Swagger 文档描述
    ENDPOINT_INFO(queryGetCategoryDetails) {
        API_DEF_ADD_TITLE(ZH_WORDS_GETTER("qi.charging.settle.summary3"));
        API_DEF_ADD_AUTH();
        // 注意：这里返回的是 JsonVO，里面包装了 Vector<DTO>
        API_DEF_ADD_RSP_JSON_WRAPPER(GetCategoryDetailsJsonVO);
        API_DEF_ADD_TAG(API_TAG);

        API_DEF_ADD_QUERY_PARAMS(String, "inpatient_no", ZH_WORDS_GETTER("qi.charging.settle.inpatient_no"), "", true);
        API_DEF_ADD_QUERY_PARAMS(String, "encounter_id", ZH_WORDS_GETTER("qi.charging.settle.encounter_id"), "", false);
    }

    // 接口定义
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/settle/getCategoryDetails",
        queryGetCategoryDetails,
        GetCategoryDetailsQuery,
        execQueryGetCategoryDetails(query)
    );

 
    GetCategoryDetailsJsonVO::Wrapper execQueryGetCategoryDetails(const GetCategoryDetailsQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif



