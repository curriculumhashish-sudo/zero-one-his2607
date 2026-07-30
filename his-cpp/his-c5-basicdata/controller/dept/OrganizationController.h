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
    /**
     * Returns the selectable organization name tree.
     *
     * Query contract:
     * - typeEnum and classEnum are optional exact-match filters.
     * - When both filters are present, they are combined with AND.
     * - typeEnum and classEnum allow at most 255 characters each.
     * - excludeId allows at most 64 characters.
     * - Only ACTIVE nodes whose complete ancestor path is ACTIVE are returned.
     * - excludeId removes the specified node and all of its descendants.
     *
     * Response contract:
     * - HTTP 200: the query succeeded; no matching node is represented by data: [].
     * - HTTP 400 / code 9995: an optional parameter exceeds its length limit.
     * - HTTP 500 / code 9994: database access, mapping, or tree assembly failed.
     * - Authentication failures are handled by the global authentication layer.
     */
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

    /**
     * Returns the organization management tree.
     *
     * Query contract:
     * - typeEnum, classEnum, and statusEnum are optional exact-match filters.
     * - keyword is optional and uses partial matching.
     * - All supplied filters are combined with AND.
     * - Each enum-code filter allows at most 255 characters.
     * - keyword allows at most 500 characters.
     * - keyword searches name, typeEnum, classEnum, statusEnum, ybNo, ybName,
     *   medinsId, medinsAdmdvs, medinsType, and medinsLv.
     * - A matching descendant is returned with its complete ancestor path.
     * - Nonmatching descendants are not added unless they match independently.
     *
     * Response contract:
     * - HTTP 200: the query succeeded; no match is represented by data: [].
     * - typeEnumDictText, classEnumDictText, and statusEnumDictText contain
     *   display values obtained from the MongoDB enum collections.
     * - If the MongoDB enum lookup fails or is incomplete, HTTP 200 and code
     *   10000 are kept, enumMappingSuccess is false, missing dictionary text
     *   fields are null, and the original enum codes are still returned.
     * - HTTP 400 / code 9995: an optional parameter exceeds its length limit.
     * - HTTP 500 / code 9994: database access, mapping, or tree assembly failed.
     * - Authentication failures are handled by the global authentication layer.
     */
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

    /**
     * Returns one non-deleted organization by its string ID.
     *
     * Query contract:
     * - id is required and must contain at most 64 characters.
     * - Leading and trailing whitespace is removed before querying.
     * - A logically deleted record is treated as nonexistent.
     *
     * Response contract:
     * - HTTP 200: the organization was found.
     * - typeEnumDictText, classEnumDictText, and statusEnumDictText contain
     *   display values obtained from the MongoDB enum collections.
     * - If the MongoDB enum lookup fails or is incomplete, HTTP 200 and code
     *   10000 are kept, enumMappingSuccess is false, missing dictionary text
     *   fields are null, and the original enum codes are still returned.
     * - HTTP 400 / code 9995: id is missing, blank, or too long.
     * - HTTP 404 / code 9999: the organization does not exist.
     * - HTTP 500 / code 9994: database access or response mapping failed.
     * - Error responses use code/message/data and set data to null.
     * - Authentication failures are handled by the global authentication layer.
     */
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
