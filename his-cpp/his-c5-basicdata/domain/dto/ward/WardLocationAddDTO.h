#pragma once
#ifndef _WARD_LOCATION_ADD_DTO_H_
#define _WARD_LOCATION_ADD_DTO_H_

#include "../../GlobalInclude.h"
#include "StringUtil.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class WardLocationAddDTO : public oatpp::DTO
{
    DTO_INIT(WardLocationAddDTO, DTO);

    DTO_FIELD(String, organizationId);
    DTO_FIELD_INFO(organizationId) {
        info->description = "Organization ID";
        info->required = true;
    }

    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = "Location name";
        info->required = true;
    }

    DTO_FIELD(String, formEnum);
    DTO_FIELD_INFO(formEnum) {
        info->description = "Location form: WARD, HOUSE or BED";
        info->required = true;
    }

    DTO_FIELD(String, parentId);
    DTO_FIELD_INFO(parentId) {
        info->description = "Parent location ID";
        info->required = false;
    }

    DTO_FIELD(String, statusEnum) = "ACTIVE";
    DTO_FIELD_INFO(statusEnum) {
        info->description = "Location status";
        info->required = false;
    }

    DTO_FIELD(String, operationalEnum) = "NOT_IN_USE";
    DTO_FIELD_INFO(operationalEnum) {
        info->description = "Operational status";
        info->required = false;
    }

    DTO_FIELD(Int32, displayOrder) = 0;
    DTO_FIELD_INFO(displayOrder) {
        info->description = "Display order";
        info->required = false;
    }

public:
    std::string validate()
    {
        if (!organizationId)
            return "organizationId is required.";

        std::string normalizedOrganizationId =
            organizationId.getValue("");

        StringUtil::trim(&normalizedOrganizationId);

        if (normalizedOrganizationId.empty())
            return "organizationId is required.";

        organizationId = normalizedOrganizationId.c_str();

        if (!name)
            return "name is required.";

        std::string normalizedName = name.getValue("");
        StringUtil::trim(&normalizedName);

        if (normalizedName.empty())
            return "name is required.";

        // Save the normalized name back into the DTO.
        name = normalizedName.c_str();

        if (!formEnum)
            return "formEnum is required.";

        std::string normalizedForm = formEnum.getValue("");
        StringUtil::trim(&normalizedForm);

        if (normalizedForm != "WARD" &&
            normalizedForm != "HOUSE" &&
            normalizedForm != "BED")
        {
            return "formEnum must be WARD, HOUSE or BED.";
        }

        formEnum = normalizedForm.c_str();

        std::string normalizedParentId =
            parentId.getValue("");

        StringUtil::trim(&normalizedParentId);

        if (normalizedForm == "WARD")
        {
            if (!normalizedParentId.empty())
                return "WARD must not have parentId.";
        }
        else
        {
            if (normalizedParentId.empty())
                return "HOUSE and BED require parentId.";

            parentId = normalizedParentId.c_str();
        }

        if (statusEnum)
        {
            std::string normalizedStatus =
                statusEnum.getValue("");

            StringUtil::trim(&normalizedStatus);

            if (normalizedStatus.empty())
                return "statusEnum must not be empty.";

            statusEnum = normalizedStatus.c_str();
        }
        else
        {
            statusEnum = "ACTIVE";
        }

        if (operationalEnum)
        {
            std::string normalizedOperational =
                operationalEnum.getValue("");

            StringUtil::trim(&normalizedOperational);

            if (normalizedOperational.empty())
                return "operationalEnum must not be empty.";

            operationalEnum = normalizedOperational.c_str();
        }
        else
        {
            operationalEnum = "NOT_IN_USE";
        }

        if (displayOrder &&
            displayOrder.getValue(0) < 0)
        {
            return "displayOrder must not be negative.";
        }

        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif