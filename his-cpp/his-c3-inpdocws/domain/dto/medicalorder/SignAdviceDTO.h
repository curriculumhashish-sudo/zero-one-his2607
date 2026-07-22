#pragma once

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

class SignAdviceItem :public oatpp::DTO {
	DTO_INIT(SignAdviceItem, DTO);
	API_DTO_FIELD_REQUIRE(String, productTableName, ZH_WORDS_GETTER("advice.field.productTableName"), true);
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("advice.field.requestId"), true);

};

class SignAdviceDTO :public oatpp::DTO {
	DTO_INIT(SignAdviceDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("advice.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("advice.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(List<SignAdviceItem::Wrapper>, signTargets, ZH_WORDS_GETTER("advice.field.signTargets"), true);
	API_DTO_FIELD_DEFAULT(String, signBatchCode, ZH_WORDS_GETTER("advice.field.signBatchCode"));
};

#include OATPP_CODEGEN_END(DTO)
