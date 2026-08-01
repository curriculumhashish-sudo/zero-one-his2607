#ifndef _PATIENTACTDTO_H_
#define _PATIENTACTDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientActDTO : public oatpp::DTO
{
	DTO_INIT(PatientActDTO, DTO);
	// Item ID (service request primary key)
	API_DTO_FIELD_REQUIRE(String, itemId, ZH_WORDS_GETTER("patient1.field.itemId"), true);
	// Request ID (parent order reference)
	API_DTO_FIELD_REQUIRE(String, requestId, ZH_WORDS_GETTER("patient1.field.requestId"), true);
	// Request type enum code (e.g. execute, skin-test, infusion)
	API_DTO_FIELD_REQUIRE(String, requestType, ZH_WORDS_GETTER("patient1.field.requestType"), true);
	// Request type display name
	API_DTO_FIELD_REQUIRE(String, requestTypeName, ZH_WORDS_GETTER("patient1.field.requestTypeName"), true);
	// Order content text
	API_DTO_FIELD_REQUIRE(String, orderContent, ZH_WORDS_GETTER("patient1.field.orderContent"), true);
	// Specification (drug/item spec)
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("patient1.field.specification"), true);
	// Status enum code
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patient1.field.status"), true);
	// Status display name
	API_DTO_FIELD_REQUIRE(String, statusName, ZH_WORDS_GETTER("patient1.field.statusName"), true);
	// Planned count (ordered quantity)
	API_DTO_FIELD_REQUIRE(Int32, plannedCount, ZH_WORDS_GETTER("patient1.field.plannedCount"), true);
	// Executed count (performed quantity)
	API_DTO_FIELD_REQUIRE(Int32, executedCount, ZH_WORDS_GETTER("patient1.field.executedCount"), true);
	// Executor display name
	API_DTO_FIELD_REQUIRE(String, executorName, ZH_WORDS_GETTER("patient1.field.executorName"), true);
	// Executed timestamp
	API_DTO_FIELD_REQUIRE(String, executedAt, ZH_WORDS_GETTER("patient1.field.executedAt"), true);
	// Version number
	API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("patient1.field.version"), true);
	// Print types (comma-separated or JSON array)
	API_DTO_FIELD_REQUIRE(String, printTypes, ZH_WORDS_GETTER("patient1.field.printTypes"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTACTDTO_H_
