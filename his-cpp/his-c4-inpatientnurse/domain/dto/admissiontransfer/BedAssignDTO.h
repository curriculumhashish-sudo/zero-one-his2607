#pragma once

#ifndef _BED_ASSIGN_DTO_
#define _BED_ASSIGN_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class BedAssignAddDTO : public oatpp::DTO
{
	DTO_INIT(BedAssignAddDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("bedassign.field.patientid"), true);
	API_DTO_FIELD_REQUIRE(String, location_id, ZH_WORDS_GETTER("bedassign.field.locationid"), true);
	API_DTO_FIELD_REQUIRE(String, appointment_id, ZH_WORDS_GETTER("bedassign.field.appointmentid"), true);
	API_DTO_FIELD_REQUIRE(String, intended_admission_date, ZH_WORDS_GETTER("bedassign.field.intended_admission_date"), true);

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->patient_id = "326940849401761792";
		dto->location_id = "BED30101";
		dto->appointment_id = "APT202607240001";
		dto->intended_admission_date = "2026-07-24";
		return dto;
	}

	std::string validate()  {
		if (!patient_id || patient_id->empty()) return "patient_id invalidate";
		else if (!location_id || location_id->empty()) return "location_id invalidate";
		else if (!appointment_id || appointment_id->empty()) return "appointment_id invalidate";
		else if (!intended_admission_date || intended_admission_date->empty()) return "intended_admission_date invalidate";

		return "";
	}
};

class BedAssignDTO : public BedAssignAddDTO
{
	DTO_INIT(BedAssignDTO, BedAssignAddDTO);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_BED_ASSIGN_DTO_
