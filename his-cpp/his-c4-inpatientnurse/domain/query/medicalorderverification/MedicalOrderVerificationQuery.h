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
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.patientId"), false);
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, locationId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.locationId"), false);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("medicalOrderVerification.field.statusEnum"), false);
	API_DTO_FIELD_REQUIRE(String, orderType,
		ZH_WORDS_GETTER("medicalOrderVerification.field.orderType"), false);
	API_DTO_FIELD_REQUIRE(String, therapyEnum,
		ZH_WORDS_GETTER("medicalOrderVerification.field.therapyEnum"), false);
	API_DTO_FIELD_REQUIRE(String, medicineCategory,
		ZH_WORDS_GETTER("medicalOrderVerification.field.medicineCategory"), false);

};

class MedicalVerificationWorkbenchQueryDTO : public oatpp::DTO
{
	DTO_INIT(MedicalVerificationWorkbenchQueryDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, encounterIds,
		ZH_WORDS_GETTER("medicalOrderVerification.field.encounterIds"), true);
	API_DTO_FIELD_REQUIRE(String, auditStatus,
		ZH_WORDS_GETTER("medicalOrderVerification.field.auditStatus"), true);
	API_DTO_FIELD_REQUIRE(String, therapyType,
		ZH_WORDS_GETTER("medicalOrderVerification.field.therapyType"), true);
	API_DTO_FIELD_REQUIRE(String, medicineCategory,
		ZH_WORDS_GETTER("medicalOrderVerification.field.medicineCategory"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->encounterIds = List<String>::createShared();
		dto->encounterIds->push_back("337907884548657152");
		dto->auditStatus = "PENDING";
		dto->therapyType = "ALL";
		dto->medicineCategory = "WEST";
		return dto;
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VERIFICATION_QUERY_H_
