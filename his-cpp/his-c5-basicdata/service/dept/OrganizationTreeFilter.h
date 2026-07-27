#ifndef _ORGANIZATIONTREEFILTER_H_
#define _ORGANIZATIONTREEFILTER_H_

#include <functional>
#include <list>

#include "domain/do/dept/adm_organizationDO.h"

class OrganizationTreeFilter
{
public:
    static std::list<PtrAdmOrganizationDO> filterWithAncestors(
        const std::list<PtrAdmOrganizationDO>& source,
        const std::function<bool(const PtrAdmOrganizationDO&)>& predicate
    );

    static std::list<PtrAdmOrganizationDO> filterSelectableWithAncestors(
        const std::list<PtrAdmOrganizationDO>& source,
        const std::function<bool(const PtrAdmOrganizationDO&)>& predicate,
        const std::string& excludeId
    );
};

#endif // !_ORGANIZATIONTREEFILTER_H_
