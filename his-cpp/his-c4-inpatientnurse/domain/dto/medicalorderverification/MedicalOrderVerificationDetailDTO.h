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
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, patientName,
		ZH_WORDS_GETTER("medicalOrderVerification.field.patientName"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("medicalOrderVerification.field.statusEnum"), true);
	API_DTO_FIELD_REQUIRE(String, therapyEnum,
		ZH_WORDS_GETTER("medicalOrderVerification.field.therapyEnum"), true);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("medicalOrderVerification.field.version"), true);
	API_DTO_FIELD_REQUIRE(String, listRowKey,
		ZH_WORDS_GETTER("medicalOrderVerification.field.listRowKey"), true);
	API_DTO_FIELD_REQUIRE(String, statusDisplay,
		ZH_WORDS_GETTER("medicalOrderVerification.field.statusDisplay"), false);
	API_DTO_FIELD_REQUIRE(String, therapyDisplay,
		ZH_WORDS_GETTER("medicalOrderVerification.field.therapyDisplay"), false);
	API_DTO_FIELD_REQUIRE(String, adviceDisplayName,
		ZH_WORDS_GETTER("medicalOrderVerification.field.adviceDisplayName"), false);
	API_DTO_FIELD_REQUIRE(String, specificationText,
		ZH_WORDS_GETTER("medicalOrderVerification.field.specificationText"), false);
	API_DTO_FIELD_REQUIRE(String, dosageDisplay,
		ZH_WORDS_GETTER("medicalOrderVerification.field.dosageDisplay"), false);
	API_DTO_FIELD_REQUIRE(String, usageDisplay,
		ZH_WORDS_GETTER("medicalOrderVerification.field.usageDisplay"), false);
	API_DTO_FIELD_REQUIRE(String, frequencyDisplay,
		ZH_WORDS_GETTER("medicalOrderVerification.field.frequencyDisplay"), false);
	API_DTO_FIELD_REQUIRE(String, authoredTime,
		ZH_WORDS_GETTER("medicalOrderVerification.field.authoredTime"), false);
	API_DTO_FIELD_REQUIRE(String, stopTime,
		ZH_WORDS_GETTER("medicalOrderVerification.field.stopTime"), false);
	API_DTO_FIELD_REQUIRE(String, requesterName,
		ZH_WORDS_GETTER("medicalOrderVerification.field.requesterName"), false);
	API_DTO_FIELD_REQUIRE(String, performerLocationDisplay,
		ZH_WORDS_GETTER("medicalOrderVerification.field.performerLocationDisplay"), false);
	API_DTO_FIELD_REQUIRE(String, categoryEnum,
		ZH_WORDS_GETTER("medicalOrderVerification.field.categoryEnum"), false);
	API_DTO_FIELD_REQUIRE(Int32, groupNo,
		ZH_WORDS_GETTER("medicalOrderVerification.field.groupNo"), false);
	API_DTO_FIELD_REQUIRE(Int32, sortNumber,
		ZH_WORDS_GETTER("medicalOrderVerification.field.sortNumber"), false);
	API_DTO_FIELD_REQUIRE(Boolean, tcmFlag,
		ZH_WORDS_GETTER("medicalOrderVerification.field.tcmFlag"), false);
	API_DTO_FIELD_REQUIRE(String, prescriptionNo,
		ZH_WORDS_GETTER("medicalOrderVerification.field.prescriptionNo"), false);
	API_DTO_FIELD_REQUIRE(List<String>, lineRequestIds,
		ZH_WORDS_GETTER("medicalOrderVerification.field.lineRequestIds"), false);
	API_DTO_FIELD_REQUIRE(Boolean, sufferingFlag,
		ZH_WORDS_GETTER("medicalOrderVerification.field.sufferingFlag"), false);
	API_DTO_FIELD_REQUIRE(Float64, chineseHerbsDoseQuantity,
		ZH_WORDS_GETTER("medicalOrderVerification.field.chineseHerbsDoseQuantity"), false);

public:
	MedicalOrderVerificationDetailDTO()
	{
		lineRequestIds = List<String>::createShared();
	}
};

class MedicalVerificationGroupDTO : public oatpp::DTO
{
	DTO_INIT(MedicalVerificationGroupDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(List<MedicalOrderVerificationDetailDTO::Wrapper>, orders,
		ZH_WORDS_GETTER("medicalOrderVerification.field.orders"), true);

public:
	MedicalVerificationGroupDTO()
	{
		orders = List<MedicalOrderVerificationDetailDTO::Wrapper>::createShared();
	}
};

class MedicalVerificationWorkbenchDTO : public oatpp::DTO
{
	DTO_INIT(MedicalVerificationWorkbenchDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<MedicalVerificationGroupDTO::Wrapper>, groups,
		ZH_WORDS_GETTER("medicalOrderVerification.field.groups"), true);

public:
	MedicalVerificationWorkbenchDTO()
	{
		groups = List<MedicalVerificationGroupDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VERIFICATION_DETAIL_DTO_H_
