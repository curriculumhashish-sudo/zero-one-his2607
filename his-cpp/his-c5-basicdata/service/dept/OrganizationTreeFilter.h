#ifndef _ORGANIZATIONTREEFILTER_H_
#define _ORGANIZATIONTREEFILTER_H_

#include <functional>
#include <list>

#include "domain/do/dept/adm_organizationDO.h"

class OrganizationTreeFilter
{
public:
    /**
     * Keeps every node accepted by predicate and adds its complete available
     * ancestor path. Descendants that do not independently match are omitted.
     * The returned list preserves the source ordering used by tree assembly.
     */
    static std::list<PtrAdmOrganizationDO> filterWithAncestors(
        const std::list<PtrAdmOrganizationDO>& source,
        const std::function<bool(const PtrAdmOrganizationDO&)>& predicate
    );

    /**
     * Builds the candidate set used by the parent-organization selector.
     * A candidate and every ancestor on its path must have ACTIVE status.
     * excludeId removes the specified node and its complete descendant set.
     * Matching nodes retain their available ancestor path and source order.
     */
    static std::list<PtrAdmOrganizationDO> filterSelectableWithAncestors(
        const std::list<PtrAdmOrganizationDO>& source,
        const std::function<bool(const PtrAdmOrganizationDO&)>& predicate,
        const std::string& excludeId
    );
};

#endif // !_ORGANIZATIONTREEFILTER_H_
