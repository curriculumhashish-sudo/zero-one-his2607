#pragma once
#ifndef _WARD_ITEM_VO_H_
#define _WARD_ITEM_VO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class WardItemVO : public oatpp::DTO
{
    DTO_INIT(WardItemVO, DTO);

    DTO_FIELD(String, id);
    DTO_FIELD(String, name);
    DTO_FIELD(String, status);
    DTO_FIELD(Int32, displayOrder);
};

#include OATPP_CODEGEN_END(DTO)

#endif