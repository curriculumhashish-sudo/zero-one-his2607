#pragma once

#ifndef _PRESCRIPTION3_BATCH_DELETE_DTO_H_
#define _PRESCRIPTION3_BATCH_DELETE_DTO_H_

#include "../../GlobalInclude.h"
#include "Prescription3OrderItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Deletes draft outpatient orders. A selected group member expands to its
 * complete group.
 */
class Prescription3BatchDeleteDTO : public oatpp::DTO
{
    DTO_INIT(Prescription3BatchDeleteDTO, DTO);

    API_DTO_FIELD_REQUIRE(
        List<Object<Prescription3OrderItemDTO>>,
        orders,
        ZH_WORDS_GETTER("prescription3.field.deleteOrders"),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif
