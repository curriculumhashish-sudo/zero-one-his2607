#ifndef _MEDICALORDERVERIFICATIONDETAILDTO_H
#define _MEDICALORDERVERIFICATIONDETAILDTO_H

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class medicalOrderVerificationDetailDTO : public oatpp::DTO
{
	DTO_INIT(medicalOrderVerificationDetailDTO, oatpp::DTO);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("medicalOrderVerification.field.name"), true);
	API_DTO_FIELD_REQUIRE(String, isPassed, ZH_WORDS_GETTER("medicalOrderVerification.field.isPassed"), true);
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("medicalOrderVerification.field.content"), true);
	API_DTO_FIELD_REQUIRE(String, type, ZH_WORDS_GETTER("medicalOrderVerification.field.type"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _MEDICALORDERVERIFICATIONDETAILDTO_H