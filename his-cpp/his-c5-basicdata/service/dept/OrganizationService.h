#ifndef _ORGANIZATIONSERVICE_H_
#define _ORGANIZATIONSERVICE_H_

#include "domain/query/dept/OrganizationNameTreeQuery.h"
#include "domain/query/dept/OrganizationListTreeQuery.h"
#include "domain/vo/dept/OrganizationTreeVO.h"
#include "domain/vo/dept/OrganizationDetailVO.h"
#include "domain/do/dept/adm_organizationDO.h"

class OrganizationService
{
private:
    // Predicates are applied before assembly so matching nodes can retain ancestors.
    bool matchesNameTreeQuery(
        const PtrAdmOrganizationDO& data,
        const OrganizationNameTreeQuery::Wrapper& query
    ) const;

    bool matchesListTreeQuery(
        const PtrAdmOrganizationDO& data,
        const OrganizationListTreeQuery::Wrapper& query
    ) const;

    bool containsIgnoreCase(
        const std::string& value,
        const std::string& keyword
    ) const;

public:
    /**
     * Returns ACTIVE selectable nodes, applies optional exact-match filters,
     * excludes the requested subtree, and preserves all valid ancestor paths.
     */
    OrganizationNameTreeJsonVO::Wrapper getNameTree(
        const OrganizationNameTreeQuery::Wrapper& query
    );

    /**
     * Applies exact filters and partial keyword matching, then returns matching
     * nodes with their ancestor paths. Nonmatching descendants are omitted.
     */
    OrganizationListTreeJsonVO::Wrapper getListTree(
        const OrganizationListTreeQuery::Wrapper& query
    );

    /**
     * Returns one non-deleted organization detail object by string ID.
     * A missing record is represented internally by a response with null data;
     * the controller converts that condition into the HTTP 404 error envelope.
     */
    OrganizationDetailJsonVO::Wrapper getDetail(
        const oatpp::String& id
    );
};

#endif // !_ORGANIZATIONSERVICE_H_
