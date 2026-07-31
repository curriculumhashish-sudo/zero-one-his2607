#include "stdafx.h"
#include "WardService.h"
#include "StringUtil.h"
#include "dao/ward/WardDAO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "picojson/picojson.h"

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

    LocationMutationResult mutationFailure(
        const std::string& message)
    {
        LocationMutationResult result;
        result.message = message;
        return result;
    }

    LocationMutationResult mutationSuccess(
        const PtrWardLocationDO& data)
    {
        LocationMutationResult result;
        result.success = true;
        result.data = data;
        return result;
    }

    std::string resolveActor(const PayloadDTO& payload)
    {
        std::string actor = payload.getUsername();
        if (actor.empty() || actor == "none")
        {
            actor = payload.getId();
        }
        return actor.empty() ? "unknown" : actor;
    }

    bool isValidJsonObject(const std::string& value)
    {
        picojson::value parsed;
        const std::string error =
            picojson::parse(parsed, value);
        return error.empty() &&
            parsed.is<picojson::object>();
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

// Query room by ID — returns id, name, wardAreaId (parent_id)
WardRoomDTO::Wrapper WardService::queryRoomById(const std::string& rawId)
{
    std::string id = rawId;
    StringUtil::trim(&id);
    if (id.empty())
        return nullptr;

    WardDAO dao;
    auto row = dao.selectLocationById(id);
    if (!row || row->getFormEnum() != "HOUSE")
        return nullptr;

    auto dto = WardRoomDTO::createShared();
    dto->id = row->getId();
    dto->name = row->getName();
    dto->wardAreaId = row->getParentId();
    return dto;
}

// Query bed by ID — returns id, name, status, wardRoomId (parent_id)
WardBedDTO::Wrapper WardService::queryBedById(const std::string& rawId)
{
    std::string id = rawId;
    StringUtil::trim(&id);
    if (id.empty())
        return nullptr;

    WardDAO dao;
    auto row = dao.selectLocationById(id);
    if (!row || row->getFormEnum() != "BED")
        return nullptr;

    auto dto = WardBedDTO::createShared();
    dto->id = row->getId();
    dto->name = row->getName();
    dto->wardRoomId = row->getParentId();
    // status: 0 = idle, 1 = in use, 2 = disabled
    if (row->getStatusEnum() == "DISABLED")
        dto->status = 2;
    else if (row->getOperationalEnum() == "IN_USE")
        dto->status = 1;
    else
        dto->status = 0;
    return dto;
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

LocationMutationResult WardService::modifyWard(
    const WardModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto)
    {
        return mutationFailure("Request body is required.");
    }

    return modifyLocation(
        dto->id.getValue(""),
        dto->name.getValue(""),
        dto->organizationId.getValue(""),
        "",
        dto->displayOrder.getValue(0),
        dto->pyStr.getValue(""),
        dto->wbStr.getValue(""),
        "",
        dto->version.getValue(-1),
        "WARD",
        payload);
}

LocationMutationResult WardService::modifyRoom(
    const RoomModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto)
    {
        return mutationFailure("Request body is required.");
    }

    return modifyLocation(
        dto->id.getValue(""),
        dto->name.getValue(""),
        dto->organizationId.getValue(""),
        dto->parentId.getValue(""),
        dto->displayOrder.getValue(0),
        dto->pyStr.getValue(""),
        dto->wbStr.getValue(""),
        "",
        dto->version.getValue(-1),
        "HOUSE",
        payload);
}

LocationMutationResult WardService::modifyBed(
    const BedModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto)
    {
        return mutationFailure("Request body is required.");
    }

    return modifyLocation(
        dto->id.getValue(""),
        dto->name.getValue(""),
        dto->organizationId.getValue(""),
        dto->parentId.getValue(""),
        dto->displayOrder.getValue(0),
        "",
        "",
        dto->typeJson.getValue("{}"),
        dto->version.getValue(-1),
        "BED",
        payload);
}

LocationMutationResult WardService::modifyLocation(
    const std::string& rawId,
    const std::string& rawName,
    const std::string& rawOrganizationId,
    const std::string& rawParentId,
    int displayOrder,
    const std::string& rawPyStr,
    const std::string& rawWbStr,
    const std::string& typeJson,
    int version,
    const std::string& formEnum,
    const PayloadDTO& payload)
{
    std::string id = rawId;
    std::string name = rawName;
    std::string organizationId = rawOrganizationId;
    std::string parentId = rawParentId;
    std::string pyStr = rawPyStr;
    std::string wbStr = rawWbStr;

    StringUtil::trim(&id);
    StringUtil::trim(&name);
    StringUtil::trim(&organizationId);
    StringUtil::trim(&parentId);
    StringUtil::trim(&pyStr);
    StringUtil::trim(&wbStr);

    if (id.empty() || name.empty() || organizationId.empty() ||
        id.size() > 64 ||
        name.size() > 255 ||
        organizationId.size() > 64 ||
        parentId.size() > 64 ||
        pyStr.size() > 255 ||
        wbStr.size() > 255 ||
        typeJson.size() > 255 ||
        version < 0 ||
        displayOrder < 0)
    {
        return mutationFailure("Invalid location modification parameters.");
    }

    if (formEnum != "WARD" && parentId.empty())
    {
        return mutationFailure("Parent location ID is required.");
    }

    if (formEnum == "BED" &&
        !isValidJsonObject(typeJson.empty() ? "{}" : typeJson))
    {
        return mutationFailure("Bed type JSON must be a valid object.");
    }

    WardDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        auto current = dao.selectLocationByIdForUpdate(id);
        if (!current || current->getFormEnum() != formEnum)
        {
            transaction.rollback();
            return mutationFailure("Location does not exist or has an unexpected type.");
        }

        if (current->getVersion() != version)
        {
            transaction.rollback();
            return mutationFailure("Location version conflict.");
        }

        const std::string targetTenantId =
            dao.selectOrganizationTenantIdForUpdate(organizationId);
        if (targetTenantId.empty() ||
            targetTenantId != current->getTenantId())
        {
            transaction.rollback();
            return mutationFailure("Organization does not belong to the location tenant.");
        }

        PtrWardLocationDO parent;
        if (formEnum != "WARD")
        {
            parent = dao.selectLocationByIdForUpdate(parentId);
            const std::string expectedParentForm =
                formEnum == "HOUSE" ? "WARD" : "HOUSE";

            if (!parent ||
                parent->getFormEnum() != expectedParentForm ||
                parent->getOrganizationId() != organizationId ||
                parent->getTenantId() != targetTenantId)
            {
                transaction.rollback();
                return mutationFailure("Parent location is invalid.");
            }

            if (parent->getStatusEnum() != "ACTIVE")
            {
                transaction.rollback();
                return mutationFailure("Parent location must be active.");
            }
        }

        const bool organizationChanged =
            current->getOrganizationId() != organizationId;
        const bool parentChanged =
            formEnum != "WARD" &&
            current->getParentId() != parentId;

        if (organizationChanged &&
            dao.hasChildren(id, false))
        {
            transaction.rollback();
            return mutationFailure("A location with children cannot change organization.");
        }

        if (formEnum == "BED" &&
            current->getOperationalEnum() == "IN_USE" &&
            (organizationChanged || parentChanged))
        {
            transaction.rollback();
            return mutationFailure("An in-use bed cannot move to another room.");
        }

        if (dao.existsSiblingName(
            organizationId,
            formEnum,
            parentId,
            name,
            id))
        {
            transaction.rollback();
            return mutationFailure("A sibling location with the same name already exists.");
        }

        current->setName(name);
        current->setOrganizationId(organizationId);
        current->setDisplayOrder(displayOrder);
        current->setUpdatedAt(SimpleDateTimeFormat::format());
        current->setUpdatedBy(resolveActor(payload));

        if (formEnum == "WARD")
        {
            current->setPyStr(pyStr);
            current->setWbStr(wbStr);
        }
        else if (formEnum == "HOUSE")
        {
            current->setParentId(parentId);
            current->setPyStr(pyStr);
            current->setWbStr(wbStr);
        }
        else
        {
            current->setParentId(parentId);
            current->setTypeJson(typeJson.empty() ? "{}" : typeJson);
        }

        if (dao.updateLocation(*current, version) != 1)
        {
            transaction.rollback();
            return mutationFailure("Location update failed because its data changed.");
        }

        current->setVersion(version + 1);
        transaction.commit();
        return mutationSuccess(current);
    }
    catch (...)
    {
        transaction.rollback();
        return mutationFailure("Location update failed.");
    }
}

LocationMutationResult WardService::removeWard(
    const std::string& id,
    const PayloadDTO& payload)
{
    return removeLocation(id, "WARD", payload);
}

LocationMutationResult WardService::removeRoom(
    const std::string& id,
    const PayloadDTO& payload)
{
    return removeLocation(id, "HOUSE", payload);
}

LocationMutationResult WardService::removeBed(
    const std::string& id,
    const PayloadDTO& payload)
{
    return removeLocation(id, "BED", payload);
}

LocationMutationResult WardService::removeLocation(
    const std::string& rawId,
    const std::string& formEnum,
    const PayloadDTO& payload)
{
    std::string id = rawId;
    StringUtil::trim(&id);

    if (id.empty() || id.size() > 64)
    {
        return mutationFailure("Location ID is required.");
    }

    WardDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        auto current = dao.selectLocationByIdForUpdate(id);
        if (!current || current->getFormEnum() != formEnum)
        {
            transaction.rollback();
            return mutationFailure("Location does not exist or has an unexpected type.");
        }

        if (dao.hasChildren(id, false))
        {
            transaction.rollback();
            return mutationFailure("A location with child locations cannot be removed.");
        }

        if (formEnum == "BED" &&
            current->getOperationalEnum() == "IN_USE")
        {
            transaction.rollback();
            return mutationFailure("An in-use bed cannot be removed.");
        }

        const std::string now = SimpleDateTimeFormat::format();
        if (dao.softDeleteLocation(
            id,
            formEnum,
            current->getVersion(),
            now,
            resolveActor(payload)) != 1)
        {
            transaction.rollback();
            return mutationFailure("Location removal failed because its data changed.");
        }

        current->setIsDeleted(true);
        current->setVersion(current->getVersion() + 1);
        transaction.commit();
        return mutationSuccess(current);
    }
    catch (...)
    {
        transaction.rollback();
        return mutationFailure("Location removal failed.");
    }
}

LocationMutationResult WardService::changeWardStatus(
    const WardStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto)
    {
        return mutationFailure("Request body is required.");
    }

    return changeLocationStatus(
        dto->id.getValue(""),
        dto->statusEnum.getValue(""),
        dto->version.getValue(-1),
        "WARD",
        payload);
}

LocationMutationResult WardService::changeRoomStatus(
    const RoomStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto)
    {
        return mutationFailure("Request body is required.");
    }

    return changeLocationStatus(
        dto->id.getValue(""),
        dto->statusEnum.getValue(""),
        dto->version.getValue(-1),
        "HOUSE",
        payload);
}

LocationMutationResult WardService::changeBedStatus(
    const BedStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    if (!dto)
    {
        return mutationFailure("Request body is required.");
    }

    return changeLocationStatus(
        dto->id.getValue(""),
        dto->statusEnum.getValue(""),
        dto->version.getValue(-1),
        "BED",
        payload);
}

LocationMutationResult WardService::changeLocationStatus(
    const std::string& rawId,
    const std::string& rawStatusEnum,
    int version,
    const std::string& formEnum,
    const PayloadDTO& payload)
{
    std::string id = rawId;
    std::string statusEnum = rawStatusEnum;
    StringUtil::trim(&id);
    StringUtil::trim(&statusEnum);

    if (id.empty() || id.size() > 64 || version < 0 ||
        (statusEnum != "ACTIVE" && statusEnum != "DISABLED"))
    {
        return mutationFailure("Invalid status modification parameters.");
    }

    WardDAO dao;
    TransactionManager transaction(&dao);

    try
    {
        auto current = dao.selectLocationByIdForUpdate(id);
        if (!current || current->getFormEnum() != formEnum)
        {
            transaction.rollback();
            return mutationFailure("Location does not exist or has an unexpected type.");
        }

        if (current->getVersion() != version)
        {
            transaction.rollback();
            return mutationFailure("Location version conflict.");
        }

        if (current->getStatusEnum() == statusEnum)
        {
            transaction.commit();
            return mutationSuccess(current);
        }

        if (statusEnum == "DISABLED")
        {
            if (dao.hasChildren(id, true))
            {
                transaction.rollback();
                return mutationFailure("Active child locations must be disabled first.");
            }

            if (formEnum == "BED" &&
                current->getOperationalEnum() == "IN_USE")
            {
                transaction.rollback();
                return mutationFailure("An in-use bed cannot be disabled.");
            }
        }
        else if (formEnum != "WARD")
        {
            auto parent =
                dao.selectLocationByIdForUpdate(current->getParentId());
            const std::string expectedParentForm =
                formEnum == "HOUSE" ? "WARD" : "HOUSE";

            if (!parent ||
                parent->getFormEnum() != expectedParentForm ||
                parent->getStatusEnum() != "ACTIVE")
            {
                transaction.rollback();
                return mutationFailure("Parent location must be active before enabling this location.");
            }
        }

        if (dao.updateLocationStatus(
            id,
            formEnum,
            statusEnum,
            version,
            SimpleDateTimeFormat::format(),
            resolveActor(payload)) != 1)
        {
            transaction.rollback();
            return mutationFailure("Status update failed because its data changed.");
        }

        current->setStatusEnum(statusEnum);
        current->setVersion(version + 1);
        transaction.commit();
        return mutationSuccess(current);
    }
    catch (...)
    {
        transaction.rollback();
        return mutationFailure("Location status update failed.");
    }
}
