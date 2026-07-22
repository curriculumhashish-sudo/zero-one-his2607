#pragma once

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

/*
*»°œ˚Õ£÷ˆDTO¿‡
*/

class CancelStopAdviceItem : public oatpp::DTO {
    DTO_INIT(CancelStopAdviceItem, DTO);
    API_DTO_FIELD_REQUIRE(String, productTableName, ZH_WORDS_GETTER("advice.field.productTableName"), true);
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("advice.field.requestId"), true);
};

class CancelStopAdviceDTO : public oatpp::DTO {
    DTO_INIT(CancelStopAdviceDTO, DTO);
    API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("advice.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("advice.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(List<CancelStopAdviceItem::Wrapper>, lines, ZH_WORDS_GETTER("advice.field.cancelStopLines"), true);
};

#include OATPP_CODEGEN_END(DTO)