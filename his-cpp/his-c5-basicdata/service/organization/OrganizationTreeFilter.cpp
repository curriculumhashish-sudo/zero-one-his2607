#include "stdafx.h"
#include "OrganizationTreeFilter.h"

#include <unordered_map>
#include <unordered_set>

std::list<PtrAdmOrganizationDO>
OrganizationTreeFilter::filterWithAncestors(
    const std::list<PtrAdmOrganizationDO>& source,
    const std::function<bool(const PtrAdmOrganizationDO&)>& predicate
)
{
    // build an index for parent lookup.
    std::unordered_map<std::string, PtrAdmOrganizationDO> nodeMap;

    for (const auto& node : source)
    {
        nodeMap[node->getId()] = node;
    }

    std::unordered_set<std::string> includedIds;
    // go one loop until the root, a missing parent, or an already visited node.
    for (const auto& node : source)
    {
        if (!predicate(node))
        {
            continue;
        }

        auto current = node;

        while (current)
        {
            const std::string currentId = current->getId();

            if (!includedIds.insert(currentId).second)
            {
                break;
            }

            const std::string parentId = current->getParent_Id();

            if (parentId.empty())
            {
                break;
            }

            auto parent = nodeMap.find(parentId);

            if (parent == nodeMap.end())
            {
                break;
            }

            current = parent->second;
        }
    }

    std::list<PtrAdmOrganizationDO> result;

    // preserve the DAO ordering in the filtered result.
    for (const auto& node : source)
    {
        if (includedIds.find(node->getId()) != includedIds.end())
        {
            result.push_back(node);
        }
    }

    return result;
}