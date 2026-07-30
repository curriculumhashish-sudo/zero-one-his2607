#include "stdafx.h"
#include "WardService.h"
#include "StringUtil.h"
#include "dao/ward/WardDAO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"

namespace
{
    using TreeNode = WardTreeNodeVO::Wrapper;
    using TreeList = oatpp::List<TreeNode>;

    bool keepByStatus(
        const TreeNode& node,
        const std::string& statusEnum)
    {
        if (!node)
            return false;

        if (statusEnum.empty())
            return true;

        const std::string nodeStatus =
            node->statusEnum.getValue("");

        if (nodeStatus != statusEnum)
            return false;

        auto keptChildren = TreeList::createShared();

        if (node->children)
        {
            for (const auto& child : *node->children)
            {
                if (keepByStatus(child, statusEnum))
                {
                    keptChildren->push_back(child);
                }
            }
        }

        node->children = keptChildren;
        return true;
    }

    bool keepByKeyword(
        const TreeNode& node,
        const std::string& keyword)
    {
        if (!node)
            return false;

        if (keyword.empty())
            return true;

        const std::string nodeName =
            node->name.getValue("");
        if (nodeName.find(keyword) != std::string::npos)
            return true;

        auto keptChildren = TreeList::createShared();

        if (node->children)
        {
            for (const auto& child : *node->children)
            {
                if (keepByKeyword(child, keyword))
                {
                    keptChildren->push_back(child);
                }
            }
        }

        node->children = keptChildren;

        return !keptChildren->empty();
    }

    TreeList applyTreeFilters(
        const TreeList& roots,
        const std::string& statusEnum,
        const std::string& keyword)
    {
        auto statusResult = TreeList::createShared();

        if (!roots)
            return statusResult;

        for (const auto& root : *roots)
        {
            if (keepByStatus(root, statusEnum))
            {
                statusResult->push_back(root);
            }
        }

        auto keywordResult = TreeList::createShared();

        for (const auto& root : *statusResult)
        {
            if (keepByKeyword(root, keyword))
            {
                keywordResult->push_back(root);
            }
        }

        return keywordResult;
    }
}

oatpp::List<WardTreeNodeVO::Wrapper> WardService::GetTree(WardTreeQuery::Wrapper query)
{
    if (!query)
        return nullptr;

    std::string organizationId =
        query->organizationId.getValue("");

    std::string wardId =
        query->wardId.getValue("");

    std::string parentId =
        query->parentId.getValue("");

    std::string keyword =
        query->keyword.getValue("");

    std::string statusEnum =
        query->statusEnum.getValue("");

    StringUtil::trim(&organizationId);
    StringUtil::trim(&wardId);
    StringUtil::trim(&parentId);
    StringUtil::trim(&keyword);
    StringUtil::trim(&statusEnum);

    if (organizationId.empty())
        return nullptr;

    if (!wardId.empty() && !parentId.empty())
        return nullptr;

    using Node = WardTreeNodeVO::Wrapper;
    std::unordered_map<std::string, Node> nodeMap;
    WardDAO dao;
    auto rows = dao.selectTreeRows(query->organizationId);
    for (const auto& row : rows) {
        auto node = WardTreeNodeVO::createShared();
        node->id = row.getId();
        node->name = row.getName();
        node->operationalEnum = row.getOperationalEnum();
        node->displayOrder = row.getDisplayOrder();
        node->statusEnum = row.getStatusEnum();
        node->parentId = row.getParentId();
        node->formEnum = row.getFormEnum();
        node->children =
            oatpp::List<Node>::createShared();

        nodeMap[row.getId()] = node;
    }
    for (const auto& row : rows) {
        if (row.getParentId().empty()) {
            continue;
        }
        auto childIt = nodeMap.find(row.getId());
        auto parentIt = nodeMap.find(row.getParentId());
        if (childIt == nodeMap.end() || parentIt == nodeMap.end())
        {
            continue;
        }
        const auto childType = row.getFormEnum();
        const auto parentType =
            parentIt->second->formEnum.getValue("");
        const bool isLeg =
            ((parentType == "house" && childType == "bed") ||
             (parentType == "ward" && childType == "house")) ||
            ((parentType == "HOUSE" && childType == "BED") ||
             (parentType == "WARD" && childType == "HOUSE"));
        if (isLeg) {
            parentIt->second->children->push_back(
                childIt->second
            );
        }
    }

    bool isleg = false;
    auto result = oatpp::List<Node>::createShared();
    if (wardId.empty() && parentId.empty())
    {
        for (const auto& row : rows)
        {
            if (row.getFormEnum() != "WARD")
                continue;

            auto it = nodeMap.find(row.getId());

            if (it != nodeMap.end())
            {
                result->push_back(it->second);
            }
        }
    }
    else if (!wardId.empty())
    {
        auto it = nodeMap.find(wardId);

        if (it == nodeMap.end())
            return nullptr;

        if (it->second->formEnum.getValue("") != "WARD")
            return nullptr;

        result->push_back(it->second);
    }
    else
    {
        auto it = nodeMap.find(parentId);

        if (it == nodeMap.end())
            return nullptr;

        const std::string parentType =
            it->second->formEnum.getValue("");

        if (parentType != "WARD" &&
            parentType != "HOUSE")
        {
            return nullptr;
        }

        if (it->second->children)
        {
            for (const auto& child : *it->second->children)
            {
                result->push_back(child);
            }
        }
    }

    return applyTreeFilters(result, statusEnum, keyword);
}

std::string WardService::createLocation(
    const WardLocationAddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto->organizationId || !dto->name || !dto->formEnum)
    {
        return "";
    }

    std::string organizationId = dto->organizationId;
    std::string name = dto->name;
    std::string formEnum = dto->formEnum;
    std::string parentId = dto->parentId ? dto->parentId->c_str() : "";

    StringUtil::trim(&organizationId);
    StringUtil::trim(&name);
    StringUtil::trim(&formEnum);
    StringUtil::trim(&parentId);

    if (organizationId.empty() || name.empty())
    {
        return "";
    }

    if (formEnum != "WARD" &&
        formEnum != "HOUSE" &&
        formEnum != "BED")
    {
        return "";
    }

    if (formEnum == "WARD" && !parentId.empty())
    {
        return "";
    }

    if ((formEnum == "HOUSE" || formEnum == "BED") &&
        parentId.empty())
    {
        return "";
    }

    WardDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        std::string tenantId =
            dao.selectOrganizationTenantIdForUpdate(organizationId);

        if (tenantId.empty())
        {
            transaction.rollback();
            return "";
        }

        PtrWardLocationDO parent;

        if (formEnum != "WARD")
        {
            parent = dao.selectLocationByIdForUpdate(parentId);

            if (!parent)
            {
                transaction.rollback();
                return "";
            }

            if (parent->getOrganizationId() != organizationId)
            {
                transaction.rollback();
                return "";
            }

            const std::string expectedParentType =
                formEnum == "HOUSE" ? "WARD" : "HOUSE";

            if (parent->getFormEnum() != expectedParentType)
            {
                transaction.rollback();
                return "";
            }

            if (parent->getTenantId() != tenantId)
            {
                transaction.rollback();
                return "";
            }
        }

        if (dao.existsSiblingName(
            organizationId,
            formEnum,
            parentId,
            name))
        {
            transaction.rollback();
            return "";
        }

        std::string id =
            std::to_string(SnowFlake::getInstance()->nextId());

        WardLocationDO data;
        data.setId(id);
        data.setTenantId(tenantId);
        data.setName(name);
        data.setFormEnum(formEnum);
        data.setOrganizationId(organizationId);

        data.setStatusEnum(
            dto->statusEnum ? dto->statusEnum->c_str() : "ACTIVE");

        data.setOperationalEnum(
            dto->operationalEnum
                ? dto->operationalEnum->c_str()
                : "NOT_IN_USE");

        data.setModeEnum("INSTANCE");
        data.setDisplayOrder(dto->displayOrder.getValue(0));
        data.setIsDeleted(false);
        data.setVersion(0);
        data.setCreatedAt(SimpleDateTimeFormat::format());

        if (formEnum != "WARD")
        {
            data.setParentId(parentId);
        }

        data.setBusNo("LO" + id);

        if (dao.insertLocation(data) != 1)
        {
            transaction.rollback();
            return "";
        }

        transaction.commit();
        return id;
    }
    catch (...)
    {
        transaction.rollback();
        return "";
    }
}


std::string WardService::validateQueryRoomById(const oatpp::String& id)
{
    if (!id || id->empty()) return "id invalidate.";
    return "";
}

PtrWardDO WardService::getRoomById(const std::string& id)
{
    WardDAO dao;
    return dao.selectRoomById(id);
}

std::string WardService::validateQueryBedById(const oatpp::String& id)
{
    if (!id || id->empty()) return "id invalidate.";
    return "";
}

PtrBedDO WardService::getBedById(const std::string& id)
{
    WardDAO dao;
    return dao.selectBedById(id);
}