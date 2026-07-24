#include "stdafx.h"
#include "WardService.h"
#include "StringUtil.h"
#include "../../dao/ward/WardDAO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "../include/id/SnowFlake.h"
#include "../../lib-common/include/SimpleDateTimeFormat.h"
#include "gmock/internal/custom/gmock-generated-actions.h"
#include"../../lib-mysql/include/TransactionManager.h"
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

        // 不传状态，不裁剪
        if (statusEnum.empty())
            return true;

        const std::string nodeStatus =
            node->statusEnum.getValue("");

        // 父节点不符合，整条子树不再检查
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

        // 第一步：状态裁剪
        for (const auto& root : *roots)
        {
            if (keepByStatus(root, statusEnum))
            {
                statusResult->push_back(root);
            }
        }

        // 第二步：名称裁剪
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
oatpp::List<WardTreeNodeVO::Wrapper>WardService::GetTree(WardTreeQuery::Wrapper query) {
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
    std::unordered_map<std::string, Node>nodeMap;
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
        //List<WardTreeNodeVO::Wrapper>
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
        const bool isLeg = (((parentType == "house") && (childType == "bed")) || ((parentType == "ward") && (childType == "house"))) ||
            (((parentType == "HOUSE") && (childType == "BED")) || ((parentType == "WARD") && (childType == "HOUSE")));
        if (isLeg) {
            parentIt->second->children->push_back(
                childIt->second
            );

        }

    }

  //  Service 返回 nullptr → 查询条件 / 节点类型非法
  //  Service 返回非空 List 但 size = 0 → 合法空结果
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
    // 2. 指定 wardId：返回这个 WARD
    else if (!wardId.empty())
    {
        auto it = nodeMap.find(wardId);

        if (it == nodeMap.end())
            return nullptr;

        if (it->second->formEnum.getValue("") != "WARD")
            return nullptr;

        result->push_back(it->second);
    }
    // 3. 指定 parentId：返回父节点的 children
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

    // 所有范围选择完成后，最后执行裁剪
    return applyTreeFilters(
        result,
        statusEnum,
        keyword
    );
}

//std::string createLocation(
//    const WardLocationAddDTO::Wrapper& dto,
//    const PayloadDTO& payload);
std::string WardService:: createLocation(
	const WardLocationAddDTO::Wrapper&dto,
	const PayloadDTO& payload)
{
	if ( !dto->organizationId || !dto->name || !dto->formEnum)
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
		// 病区不能有父级
		return "";
	}

	if ((formEnum == "HOUSE" || formEnum == "BED") &&
		parentId.empty())
	{
		// 病房、床位必须有父级
		return "";
	}

	WardDAO dao;
	TransactionManager transaction(&dao);

	try
	{
		// 三种类型都必须确认机构存在
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
				return ""; // 上级位置不存在
			}

			if (parent->getOrganizationId() != organizationId)
			{
				transaction.rollback();
				return ""; // 上级机构不一致
			}

			const std::string expectedParentType =
				formEnum == "HOUSE" ? "WARD" : "HOUSE";

			if (parent->getFormEnum() != expectedParentType)
			{
				transaction.rollback();
				return ""; // 父级类型错误
			}

			if (parent->getTenantId() != tenantId)
			{
				transaction.rollback();
				return ""; // 租户不一致
			}
		}

		if (dao.existsSiblingName(
			organizationId,
			formEnum,
			parentId,
			name))
		{
			transaction.rollback();
			return ""; // 同级名称重复
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

		// 病区不要写空字符串，应让 parent_id 存 SQL NULL
		if (formEnum != "WARD")
		{
			data.setParentId(parentId);
		}

		// adm_location.bus_no 非空，必须按项目规则生成
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



















