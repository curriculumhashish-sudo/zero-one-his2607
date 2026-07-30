#ifndef _MEDICAL_ORDER_VERIFICATION_DETAIL_DTO_H_
#define _MEDICAL_ORDER_VERIFICATION_DETAIL_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicalOrderVerificationDetailDTO : public oatpp::DTO
{
	DTO_INIT(MedicalOrderVerificationDetailDTO, oatpp::DTO);
	API_DTO_FIELD_REQUIRE(String, requestId, ZH_WORDS_GETTER("medicalOrderVerification.field.requestId"), true);
	API_DTO_FIELD_REQUIRE(String, orderType, ZH_WORDS_GETTER("medicalOrderVerification.field.orderType"), true);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("medicalOrderVerification.field.name"), true);
	API_DTO_FIELD_REQUIRE(String, isPassed, ZH_WORDS_GETTER("medicalOrderVerification.field.isPassed"), true);
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("medicalOrderVerification.field.content"), true);
	API_DTO_FIELD_REQUIRE(String, type, ZH_WORDS_GETTER("medicalOrderVerification.field.type"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VERIFICATION_DETAIL_DTO_H_
