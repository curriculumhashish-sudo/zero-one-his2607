#pragma once

#ifndef _PRESCRIPTION3_MERGE_GROUP_DTO_H_
#define _PRESCRIPTION3_MERGE_GROUP_DTO_H_

#include "../../GlobalInclude.h"
#include "Prescription3OrderItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Merges two or more draft order units. Existing groups are expanded and
 * replaced by one server-generated group identifier.
 */
class Prescription3MergeGroupDTO : public oatpp::DTO
{
    DTO_INIT(Prescription3MergeGroupDTO, DTO);

    API_DTO_FIELD_REQUIRE(
        List<Object<Prescription3OrderItemDTO>>,
        orders,
        ZH_WORDS_GETTER("prescription3.field.mergeOrders"),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif
