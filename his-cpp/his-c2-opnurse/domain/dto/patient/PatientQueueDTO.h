#ifndef _PATIENTQUEUEDTO_H_
#define _PATIENTQUEUEDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientQueueDTO : public oatpp::DTO
{
	DTO_INIT(PatientQueueDTO, DTO);
	// Encounter ID
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("patient1.field.encounterId"), true);
	// Patient ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("patient1.field.patientId"), true);
	// Patient display name
	API_DTO_FIELD_REQUIRE(String, patientName, ZH_WORDS_GETTER("patient1.field.patientName"), true);
	// Gender display name (e.g. male/female in Chinese)
	API_DTO_FIELD_REQUIRE(String, genderName, ZH_WORDS_GETTER("patient1.field.genderName"), true);
	// Age text (e.g. "28岁")
	API_DTO_FIELD_REQUIRE(String, ageText, ZH_WORDS_GETTER("patient1.field.ageText"), true);
	// Medical record number
	API_DTO_FIELD_REQUIRE(String, medicalRecordNo, ZH_WORDS_GETTER("patient1.field.medicalRecordNo"), true);
	// Visit number
	API_DTO_FIELD_REQUIRE(String, visitNo, ZH_WORDS_GETTER("patient1.field.visitNo"), true);
	// Queue number (business number)
	API_DTO_FIELD_REQUIRE(String, queueNumber, ZH_WORDS_GETTER("patient1.field.queueNumber"), true);
	// Registered date/time
	API_DTO_FIELD_REQUIRE(String, registeredAt, ZH_WORDS_GETTER("patient1.field.registeredAt"), true);
	// Department display name
	API_DTO_FIELD_REQUIRE(String, departmentName, ZH_WORDS_GETTER("patient1.field.departmentName"), true);
	// Visit type display name
	API_DTO_FIELD_REQUIRE(String, visitTypeName, ZH_WORDS_GETTER("patient1.field.visitTypeName"), true);
	// Status enum code (e.g. waiting, called, completed)
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patient1.field.status"), true);
	// Status display name
	API_DTO_FIELD_REQUIRE(String, statusName, ZH_WORDS_GETTER("patient1.field.statusName"), true);
	// Payment type display name
	API_DTO_FIELD_REQUIRE(String, paymentTypeName, ZH_WORDS_GETTER("patient1.field.paymentTypeName"), true);
	// Pending count (number of patients ahead in queue)
	API_DTO_FIELD_REQUIRE(Int32, pendingCount, ZH_WORDS_GETTER("patient1.field.pendingCount"), true);
	// Priority display name (triage level display)
	API_DTO_FIELD_REQUIRE(String, priorityName, ZH_WORDS_GETTER("patient1.field.priorityName"), true);
	// Risk hint (e.g. allergy, special notes)
	API_DTO_FIELD_REQUIRE(String, riskHint, ZH_WORDS_GETTER("patient1.field.riskHint"), true);
};

/**
 * Paginated DTO for patient queue list.
 */
class PatientQueuePageDTO : public PageDTO<PatientQueueDTO::Wrapper>
{
	DTO_INIT(PatientQueuePageDTO, PageDTO<PatientQueueDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTQUEUEDTO_H_
