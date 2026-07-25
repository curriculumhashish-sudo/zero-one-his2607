#pragma once
#ifndef _GET_CATEGORY_DETAILS_QUERY_
#define _GET_CATEGORY_DETAILS_QUERY_

#include "../../GlobalInclude.h" 
#include OATPP_CODEGEN_BEGIN(DTO)

class GetCategoryDetailsQuery : public oatpp::DTO {
	DTO_INIT(GetCategoryDetailsQuery, DTO)

	DTO_FIELD(String, inpatient_no);
	DTO_FIELD(String, encounter_id);
};

#include OATPP_CODEGEN_END(DTO)
#endif


