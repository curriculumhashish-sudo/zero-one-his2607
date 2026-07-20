#pragma once

#ifndef _GET_TOTAL_FEES_CONTROLLER_
#define _GET_TOTAL_FEES_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/settle/GetTotalFeesQuery.h"
#include "domain/vo/settle/GetTotalFeesVO.h"
#include "domain/DTO/settle/GetTotalFeesDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("qi.charging.settle.tag2")

class GetTotalFeesController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(GetTotalFeesController);
public:
   
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("qi.charging.settle.summary2"),
        queryGetTotalFees,
        GetTotalFeesQuery,
        GetTotalFeesJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/settle/getTotalFees",
        queryGetTotalFees,
        GetTotalFeesQuery,
        execQueryGetTotalFees(query)
    );

private:
    
    GetTotalFeesJsonVO::Wrapper execQueryGetTotalFees(const GetTotalFeesQuery::Wrapper& query);
};


#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif