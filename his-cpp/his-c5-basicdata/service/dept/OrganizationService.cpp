#include "stdafx.h"
#include "OrganizationService.h"

#include <algorithm>
#include <cctype>
#include <vector>

#include "dao/dept/OrganizationDAO.h"
#include "OrganizationTreeFilter.h"
#include "OrganizationTreeAssembler.h"
#include "OrganizationNameTreeMapper.h"
#include "OrganizationListTreeMapper.h"
#include "OrganizationEnumDictionary.h"

namespace
{
    bool hasMissingEnumMapping(
        const PtrAdmOrganizationDO& data,
        const OrganizationEnumTextMap& mappings
    )
    {
        return
            (!data->getTypeEnum().empty() &&
                !mappings.hasTypeText(data->getTypeEnum())) ||
            (!data->getClassEnum().empty() &&
                !mappings.hasClassText(data->getClassEnum())) ||
            (!data->getStatusEnum().empty() &&
                !mappings.hasStatusText(data->getStatusEnum()));
    }

    const char* getEnumWarningKey(OrganizationEnumLoadStatus status)
    {
        return status == OrganizationEnumLoadStatus::MONGO_UNAVAILABLE
            ? "dept.organizationQuery.warnings.enumMongoUnavailable"
            : "dept.organizationQuery.warnings.enumDataIncomplete";
    }
}

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
    const std::string typeEnum = query->typeEnum.getValue("");
    const std::string classEnum = query->classEnum.getValue("");

    if (!typeEnum.empty() && data->getTypeEnum() != typeEnum)
    {
        return false;
    }

    if (!classEnum.empty() && data->getClassEnum() != classEnum)
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
    const std::string typeEnum = query->typeEnum.getValue("");
    const std::string classEnum = query->classEnum.getValue("");
    const std::string statusEnum = query->statusEnum.getValue("");
    const std::string keyword = query->keyword.getValue("");

    if (!typeEnum.empty() && data->getTypeEnum() != typeEnum)
    {
        return false;
    }

    if (!classEnum.empty() && data->getClassEnum() != classEnum)
    {
        return false;
    }

    if (!statusEnum.empty() && data->getStatusEnum() != statusEnum)
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
        data->getTypeEnum(),
        data->getClassEnum(),
        data->getStatusEnum(),
        data->getYbNo(),
        data->getYbName(),
        data->getMedinsId(),
        data->getMedinsAdmdvs(),
        data->getMedinsType(),
        data->getMedinsLv()
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
        OrganizationTreeFilter::filterSelectableWithAncestors(
            allOrganizations,
            [this, &query](const PtrAdmOrganizationDO& data)
            {
                return matchesNameTreeQuery(data, query);
            },
            query->excludeId.getValue("")
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

    const auto enumResult = OrganizationEnumDictionary().load();
    auto enumStatus = enumResult.status;

    if (enumStatus != OrganizationEnumLoadStatus::MONGO_UNAVAILABLE)
    {
        for (const auto& organization : filteredOrganizations)
        {
            if (hasMissingEnumMapping(organization, enumResult.mappings))
            {
                enumStatus =
                    OrganizationEnumLoadStatus::ENUM_DATA_INCOMPLETE;
                break;
            }
        }
    }

    const auto roots =
        OrganizationTreeAssembler::build(
            filteredOrganizations,
            OrganizationListTreeMapper(enumResult.mappings)
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
    result->enumMappingSuccess =
        enumStatus == OrganizationEnumLoadStatus::SUCCESS;

    if (enumStatus != OrganizationEnumLoadStatus::SUCCESS)
    {
        result->message = ZH_WORDS_GETTER(
            getEnumWarningKey(enumStatus)
        );
    }

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

    const auto enumResult = OrganizationEnumDictionary().load();
    auto enumStatus = enumResult.status;

    if (enumStatus != OrganizationEnumLoadStatus::MONGO_UNAVAILABLE &&
        hasMissingEnumMapping(data, enumResult.mappings))
    {
        enumStatus = OrganizationEnumLoadStatus::ENUM_DATA_INCOMPLETE;
    }

    auto detail = OrganizationDetailDTO::createShared();

    detail->id = data->getId();
    detail->name = data->getName();
    if (!data->getParentId().empty())
    {
        detail->parentId = data->getParentId();
    }

    if (!data->getDefDoctorId().empty())
    {
        detail->defDoctorId = data->getDefDoctorId();
    }

    detail->busNo = data->getBusNo();
    detail->caty = data->getCaty();
    detail->typeEnum = data->getTypeEnum();
    detail->typeEnumDictText = enumResult.mappings.getTypeText(
        data->getTypeEnum()
    );
    detail->classEnum = data->getClassEnum();
    detail->classEnumDictText = enumResult.mappings.getClassText(
        data->getClassEnum()
    );
    detail->statusEnum = data->getStatusEnum();
    detail->statusEnumDictText = enumResult.mappings.getStatusText(
        data->getStatusEnum()
    );
    detail->ybNo = data->getYbNo();
    detail->ybName = data->getYbName();
    detail->medinsId = data->getMedinsId();
    detail->medinsAdmdvs = data->getMedinsAdmdvs();
    detail->medinsType = data->getMedinsType();
    detail->medinsLv = data->getMedinsLv();
    detail->pyStr = data->getPyStr();
    detail->wbStr = data->getWbStr();
    detail->displayOrder = data->getDisplayOrder();

    result->success(detail);
    result->enumMappingSuccess =
        enumStatus == OrganizationEnumLoadStatus::SUCCESS;

    if (enumStatus != OrganizationEnumLoadStatus::SUCCESS)
    {
        result->message = ZH_WORDS_GETTER(
            getEnumWarningKey(enumStatus)
        );
    }

    return result;
}
