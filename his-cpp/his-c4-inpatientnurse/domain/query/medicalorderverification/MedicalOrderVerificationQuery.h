#pragma once

#ifndef _MEDICAL_ORDER_VERIFICATION_QUERY_H_
#define _MEDICAL_ORDER_VERIFICATION_QUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicalVerificationQuery : public oatpp::DTO
{
	DTO_INIT(MedicalVerificationQuery, oatpp::DTO);

	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("medicalOrderVerification.field.name"), false);
	API_DTO_FIELD_REQUIRE(String, isPassed, ZH_WORDS_GETTER("medicalOrderVerification.field.isPassed"), false);
	API_DTO_FIELD_REQUIRE(String, medicalType, ZH_WORDS_GETTER("medicalOrderVerification.field.medicalType"), false);
	API_DTO_FIELD_REQUIRE(String, medicalPeriod, ZH_WORDS_GETTER("medicalOrderVerification.field.medicalPeriod"), false);

public:
	MedicalVerificationQuery()
	{
		name = ZH_WORDS_GETTER("business-text.sample.person-name");
		isPassed = "false";
		medicalType = "MED_REQUEST";
		medicalPeriod = "TEMPORARY";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VERIFICATION_QUERY_H_
