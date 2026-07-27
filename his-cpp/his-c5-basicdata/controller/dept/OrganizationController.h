#ifndef _ORGANIZATIONCONTROLLER_H_
#define _ORGANIZATIONCONTROLLER_H_

#include "ApiHelper.h"
#include "Macros.h"
using namespace oatpp;
#include "domain/query/dept/OrganizationNameTreeQuery.h"
#include "domain/query/dept/OrganizationListTreeQuery.h"
#include "domain/vo/dept/OrganizationTreeVO.h"
#include "domain/vo/dept/OrganizationDetailVO.h"
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

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/dept/name-tree",
        queryNameTree,
        OrganizationNameTreeQuery,
        executeQueryNameTree(query)
    );

    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("dept.organizationQuery.endpoints.listTree"),
        queryListTree,
        OrganizationListTreeQuery,
        OrganizationListTreeJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/dept/tree",
        queryListTree,
        OrganizationListTreeQuery,
        executeQueryListTree(query)
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

    API_HANDLER_ENDPOINT_AUTH(
        API_M_GET,
        "/dept/detail",
        queryDetail,
        QUERY(oatpp::String, id),
        executeQueryDetail(id)
    );

private:
    OrganizationNameTreeJsonVO::Wrapper executeQueryNameTree(
        const OrganizationNameTreeQuery::Wrapper& query
    );

    OrganizationListTreeJsonVO::Wrapper executeQueryListTree(
        const OrganizationListTreeQuery::Wrapper& query
    );

    OrganizationDetailJsonVO::Wrapper executeQueryDetail(
        const oatpp::String& id
    );
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_ORGANIZATIONCONTROLLER_H_
