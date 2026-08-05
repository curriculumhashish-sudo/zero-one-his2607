#pragma once

#ifndef _WITHDRAWMEDICALADVICEDTO_H_
#define _WITHDRAWMEDICALADVICEDTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class WithdrawMedicalAdviceDTO : public oatpp::DTO
{
    DTO_INIT(WithdrawMedicalAdviceDTO, DTO);

    // Encounter identifier containing the medical advice records.
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("medical-advice.field.encounter-id"), true);
    // Expected optimistic lock version.
    API_DTO_FIELD_REQUIRE(Int32, version,
        ZH_WORDS_GETTER("medical-advice.field.version"), true);
    // Medical advice identifiers to withdraw.
    API_DTO_FIELD_REQUIRE(List<String>, adviceIds,
        ZH_WORDS_GETTER("medical-advice.field.advice-ids"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_WITHDRAWMEDICALADVICEDTO_H_
