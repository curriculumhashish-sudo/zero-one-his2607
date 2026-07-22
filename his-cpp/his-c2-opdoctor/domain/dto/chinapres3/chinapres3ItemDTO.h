#pragma once

#ifndef _chinapres3ITEMDTO_H_
#define _chinapres3ITEMDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * ´¦·½Ò©²ÄDTO
 */
class chinapres3ItemDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3ItemDTO, DTO);

    // Ò©²ÄID
    API_DTO_FIELD_REQUIRE(String, medicineId, ZH_WORDS_GETTER("chinapres3.field.medicineId"), true);

    // ¼ÁÁ¿(g)
    API_DTO_FIELD_REQUIRE(Float64, dosage, ZH_WORDS_GETTER("chinapres3.field.dosage"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3ITEMDTO_H_