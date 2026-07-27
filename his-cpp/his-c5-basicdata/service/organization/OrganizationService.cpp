#include "stdafx.h"
#include "OrganizationService.h"

#include <algorithm>
#include <cctype>
#include <vector>

#include "dao/organization/OrganizationDAO.h"
#include "OrganizationTreeFilter.h"
#include "OrganizationTreeAssembler.h"
#include "OrganizationNameTreeMapper.h"
#include "OrganizationListTreeMapper.h"

bool OrganizationService::containsIgnoreCase(
    const std::string& value,
    const std::string& keyword
) const
{
    if (keyword.empty())
    {
        return true;
    }

    std::string normalizedValue = value;
    std::string normalizedKeyword = keyword;

    std::transform(
        normalizedValue.begin(),
        normalizedValue.end(),
        normalizedValue.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(std::tolower(character));
        }
    );

    std::transform(
        normalizedKeyword.begin(),
        normalizedKeyword.end(),
        normalizedKeyword.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(std::tolower(character));
        }
    );

    return normalizedValue.find(normalizedKeyword) != std::string::npos;
}

bool OrganizationService::matchesNameTreeQuery(
    const PtrAdmOrganizationDO& data,
    const OrganizationNameTreeQuery::Wrapper& query
) const
{
    // Type and category filters are combined with AND.
    const std::string typeEnum = query->type_enum.getValue("");
    const std::string classEnum = query->class_enum.getValue("");

    if (!typeEnum.empty() && data->getType_Enum() != typeEnum)
    {
        return false;
    }

    if (!classEnum.empty() && data->getClass_Enum() != classEnum)
    {
        return false;
    }

    return true;
}

bool OrganizationService::matchesListTreeQuery(
    const PtrAdmOrganizationDO& data,
    const OrganizationListTreeQuery::Wrapper& query
) const
{
    const std::string typeEnum = query->type_enum.getValue("");
    const std::string classEnum = query->class_enum.getValue("");
    const std::string statusEnum = query->status_enum.getValue("");
    const std::string keyword = query->keyword.getValue("");

    if (!typeEnum.empty() && data->getType_Enum() != typeEnum)
    {
        return false;
    }

    if (!classEnum.empty() && data->getClass_Enum() != classEnum)
    {
        return false;
    }

    if (!statusEnum.empty() && data->getStatus_Enum() != statusEnum)
    {
        return false;
    }

    if (keyword.empty())
    {
        return true;
    }

    const std::vector<std::string> searchableFields =
    {
        data->getName(),
        data->getType_Enum(),
        data->getClass_Enum(),
        data->getStatus_Enum(),
        data->getYb_No(),
        data->getYb_Name(),
        data->getMedins_Id(),
        data->getMedins_Admdvs(),
        data->getMedins_Type(),
        data->getMedins_Lv()
    };

    for (const auto& field : searchableFields)
    {
        if (containsIgnoreCase(field, keyword))
        {
            return true;
        }
    }

    return false;
}

OrganizationNameTreeJsonVO::Wrapper OrganizationService::getNameTree(
    const OrganizationNameTreeQuery::Wrapper& query
)
{
    OrganizationDAO organizationDAO;

    const auto allOrganizations =
        organizationDAO.selectAllNotDeleted();

    const auto filteredOrganizations =
        OrganizationTreeFilter::filterWithAncestors(
            allOrganizations,
            [this, &query](const PtrAdmOrganizationDO& data)
            {
                return matchesNameTreeQuery(data, query);
            }
        );

    const auto roots =
        OrganizationTreeAssembler::build(
            filteredOrganizations,
            OrganizationNameTreeMapper()
        );

    auto treeData =
        oatpp::List<OrganizationNameTreeDTO::Wrapper>::createShared();

    for (const auto& rootNode : roots)
    {
        auto root =
            std::dynamic_pointer_cast<OrganizationNameTreeDTO>(rootNode);

        if (root)
        {
            // Convert each internal root node into the name-tree response type.
            treeData->push_back(
                OrganizationNameTreeDTO::Wrapper(
                    root,
                    OrganizationNameTreeDTO::Wrapper::Class::getType()
                )
            );
        }
    }

    auto result = OrganizationNameTreeJsonVO::createShared();
    result->success(treeData);

    return result;
}

OrganizationListTreeJsonVO::Wrapper OrganizationService::getListTree(
    const OrganizationListTreeQuery::Wrapper& query
)
{
    OrganizationDAO organizationDAO;

    const auto allOrganizations =
        organizationDAO.selectAllNotDeleted();

    const auto filteredOrganizations =
        OrganizationTreeFilter::filterWithAncestors(
            allOrganizations,
            [this, &query](const PtrAdmOrganizationDO& data)
            {
                return matchesListTreeQuery(data, query);
            }
        );

    const auto roots =
        OrganizationTreeAssembler::build(
            filteredOrganizations,
            OrganizationListTreeMapper()
        );

    auto treeData =
        oatpp::List<OrganizationListTreeDTO::Wrapper>::createShared();

    for (const auto& rootNode : roots)
    {
        auto root =
            std::dynamic_pointer_cast<OrganizationListTreeDTO>(rootNode);

        if (root)
        {
            // Convert each internal root node into the list-tree response type.
            treeData->push_back(
                OrganizationListTreeDTO::Wrapper(
                    root,
                    OrganizationListTreeDTO::Wrapper::Class::getType()
                )
            );
        }
    }

    auto result = OrganizationListTreeJsonVO::createShared();
    result->success(treeData);

    return result;
}

OrganizationDetailJsonVO::Wrapper OrganizationService::getDetail(
    const oatpp::String& id
)
{
    OrganizationDAO organizationDAO;

    const auto data =
        organizationDAO.selectById(id.getValue(""));

    auto result = OrganizationDetailJsonVO::createShared();

    if (!data)
    {
        result->fail(nullptr);
        return result;
    }

    auto detail = OrganizationDetailDTO::createShared();

    detail->id = data->getId();
    detail->name = data->getName();
    detail->type_enum = data->getType_Enum();
    detail->class_enum = data->getClass_Enum();
    detail->status_enum = data->getStatus_Enum();
    detail->yb_no = data->getYb_No();
    detail->yb_name = data->getYb_Name();
    detail->medins_id = data->getMedins_Id();
    detail->medins_admdvs = data->getMedins_Admdvs();
    detail->medins_type = data->getMedins_Type();
    detail->medins_lv = data->getMedins_Lv();
    detail->py_str = data->getPy_Str();
    detail->wb_str = data->getWb_Str();
    detail->display_order = data->getDisplay_Order();

    result->success(detail);

    return result;
}