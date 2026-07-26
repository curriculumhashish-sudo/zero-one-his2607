#pragma once

#ifndef _PRESCRIPTION3_SPLIT_GROUP_DTO_H_
#define _PRESCRIPTION3_SPLIT_GROUP_DTO_H_

#include "../../GlobalInclude.h"
#include "Prescription3OrderItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Splits complete draft groups. Passing any group member selects its complete
 * group.
 */
class Prescription3SplitGroupDTO : public oatpp::DTO
{
    DTO_INIT(Prescription3SplitGroupDTO, DTO);

    API_DTO_FIELD_REQUIRE(
        List<Object<Prescription3OrderItemDTO>>,
        orders,
        ZH_WORDS_GETTER("prescription3.field.splitOrders"),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif
