#ifndef _PATIENTACTDTO_H_
#define _PATIENTACTDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientActDTO : public oatpp::DTO
{
	DTO_INIT(PatientActDTO, DTO);
	// Status
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("patient.field.status"), true);
	// Order content
	API_DTO_FIELD_REQUIRE(String, orderContent, ZH_WORDS_GETTER("patient.field.orderContent"), true);
	// Whether executed
	API_DTO_FIELD_REQUIRE(Int32, isAct, ZH_WORDS_GETTER("patient.field.act"), false);
};

/**
 * Paginated DTO for patient list.
 */
class PatientActPageDTO : public PageDTO<PatientActDTO::Wrapper>
{
	DTO_INIT(PatientActPageDTO, PageDTO<PatientActDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENTACTDTO_H_
