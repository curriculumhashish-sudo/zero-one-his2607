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
    // Query predicates are separated from DAO code to preserve ancestor paths.
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
    OrganizationNameTreeJsonVO::Wrapper getNameTree(
        const OrganizationNameTreeQuery::Wrapper& query
    );

    OrganizationListTreeJsonVO::Wrapper getListTree(
        const OrganizationListTreeQuery::Wrapper& query
    );

    OrganizationDetailJsonVO::Wrapper getDetail(
        const oatpp::String& id
    );
};

#endif // !_ORGANIZATIONSERVICE_H_
