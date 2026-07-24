#pragma once
#ifndef _WARD_LOCATION_ADD_DTO_H_
#define _WARD_LOCATION_ADD_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class WardLocationAddDTO : public oatpp::DTO
{
    DTO_INIT(WardLocationAddDTO, DTO);

    DTO_FIELD(String, organizationId);
    DTO_FIELD(String, name);
    DTO_FIELD(String, formEnum);
    DTO_FIELD(String, parentId);

    DTO_FIELD(String, statusEnum) = "ACTIVE";
    DTO_FIELD(String, operationalEnum) = "NOT_IN_USE";
    DTO_FIELD(Int32, displayOrder) = 0;
};

#include OATPP_CODEGEN_END(DTO)

#endif
