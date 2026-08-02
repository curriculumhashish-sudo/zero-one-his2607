#include "stdafx.h"
#include "OrganizationTreeFilter.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

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

            const std::string parentId = current->getParentId();

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

std::list<PtrAdmOrganizationDO>
OrganizationTreeFilter::filterSelectableWithAncestors(
    const std::list<PtrAdmOrganizationDO>& source,
    const std::function<bool(const PtrAdmOrganizationDO&)>& predicate,
    const std::string& excludeId
)
{
    std::unordered_map<std::string, PtrAdmOrganizationDO> nodeMap;
    std::unordered_map<std::string, std::vector<PtrAdmOrganizationDO>> childrenMap;

    for (const auto& node : source)
    {
        nodeMap[node->getId()] = node;
        childrenMap[node->getParentId()].push_back(node);
    }

    std::unordered_set<std::string> excludedIds;

    if (!excludeId.empty())
    {
        std::vector<std::string> pendingIds = { excludeId };

        while (!pendingIds.empty())
        {
            const std::string currentId = pendingIds.back();
            pendingIds.pop_back();

            if (!excludedIds.insert(currentId).second)
            {
                continue;
            }

            auto children = childrenMap.find(currentId);

            if (children == childrenMap.end())
            {
                continue;
            }

            for (const auto& child : children->second)
            {
                pendingIds.push_back(child->getId());
            }
        }
    }

    std::unordered_set<std::string> includedIds;

    for (const auto& node : source)
    {
        if (node->getStatusEnum() != "ACTIVE" ||
            excludedIds.find(node->getId()) != excludedIds.end() ||
            !predicate(node))
        {
            continue;
        }

        std::vector<std::string> pathIds;
        std::unordered_set<std::string> pathVisited;
        auto current = node;
        bool isSelectablePath = true;

        while (current)
        {
            const std::string currentId = current->getId();

            if (!pathVisited.insert(currentId).second ||
                current->getStatusEnum() != "ACTIVE" ||
                excludedIds.find(currentId) != excludedIds.end())
            {
                isSelectablePath = false;
                break;
            }

            pathIds.push_back(currentId);

            const std::string parentId = current->getParentId();

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

        if (!isSelectablePath)
        {
            continue;
        }

        includedIds.insert(pathIds.begin(), pathIds.end());
    }

    std::list<PtrAdmOrganizationDO> result;

    for (const auto& node : source)
    {
        if (includedIds.find(node->getId()) != includedIds.end())
        {
            result.push_back(node);
        }
    }

    return result;
}
