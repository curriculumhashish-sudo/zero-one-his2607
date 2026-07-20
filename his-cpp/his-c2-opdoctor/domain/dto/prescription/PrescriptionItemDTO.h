#pragma once

#ifndef _PRESCRIPTIONITEMDTO_H_
#define _PRESCRIPTIONITEMDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * ´¦·½Ò©²ÄDTO
 */
class PrescriptionItemDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionItemDTO, DTO);

    // Ò©²ÄID
    API_DTO_FIELD_REQUIRE(String, medicineId, ZH_WORDS_GETTER("prescription.field.medicineId"), true);

    // ¼ÁÁ¿(g)
    API_DTO_FIELD_REQUIRE(Float64, dosage, ZH_WORDS_GETTER("prescription.field.dosage"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTIONITEMDTO_H_