#pragma once
#ifndef _WARD_LIST_QUERY_H_
#define _WARD_LIST_QUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class WardListQuery : public oatpp::DTO
{
    DTO_INIT(WardListQuery, DTO);

    DTO_FIELD(String, organizationId);
    DTO_FIELD_INFO(organizationId) {
        info->description = "Organization ID";
        info->required = true;
    }

    DTO_FIELD(String, keyword);
    DTO_FIELD_INFO(keyword) {
        info->description = "Ward name keyword";
    }

    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = "Ward status";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif