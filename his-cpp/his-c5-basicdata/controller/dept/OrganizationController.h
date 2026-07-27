#ifndef _ORGANIZATIONCONTROLLER_H_
#define _ORGANIZATIONCONTROLLER_H_

#include "ApiHelper.h"
#include "Macros.h"
using namespace oatpp;
#include "domain/query/dept/OrganizationNameTreeQuery.h"
#include "domain/query/dept/OrganizationListTreeQuery.h"
#include "domain/vo/dept/OrganizationTreeVO.h"
#include "domain/vo/dept/OrganizationDetailVO.h"
#include "domain/vo/dept/OrganizationQueryErrorVO.h"
#include "service/dept/OrganizationService.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("dept.tag")

class OrganizationController
    : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(OrganizationController);

public:
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("dept.organizationQuery.endpoints.nameTree"),
        queryNameTree,
        OrganizationNameTreeQuery,
        OrganizationNameTreeJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_GET,
        "/dept/name-tree",
        queryNameTree,
        QUERIES(QueryParams, queryParams),
        API_HANDLER_QUERY_PARAM(
            query,
            OrganizationNameTreeQuery,
            queryParams
        );
        return executeQueryNameTree(query);
    );

    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("dept.organizationQuery.endpoints.listTree"),
        queryListTree,
        OrganizationListTreeQuery,
        OrganizationListTreeJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_GET,
        "/dept/tree",
        queryListTree,
        QUERIES(QueryParams, queryParams),
        API_HANDLER_QUERY_PARAM(
            query,
            OrganizationListTreeQuery,
            queryParams
        );
        return executeQueryListTree(query);
    );

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("dept.organizationQuery.endpoints.detail"),
        queryDetail,
        OrganizationDetailJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "id",
            ZH_WORDS_GETTER("dept.organizationQuery.queries.id"),
            "322663218269917184",
            true
        );
    );

    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_GET,
        "/dept/detail",
        queryDetail,
        QUERY(oatpp::String, id),
        return executeQueryDetail(id);
    );

private:
    std::shared_ptr<OutgoingResponse> executeQueryNameTree(
        const OrganizationNameTreeQuery::Wrapper& query
    );

    std::shared_ptr<OutgoingResponse> executeQueryListTree(
        const OrganizationListTreeQuery::Wrapper& query
    );

    std::shared_ptr<OutgoingResponse> executeQueryDetail(
        const oatpp::String& id
    );

    std::shared_ptr<OutgoingResponse> createErrorResponse(
        const Status& status,
        int code,
        const std::string& messageKey
    );
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_ORGANIZATIONCONTROLLER_H_
