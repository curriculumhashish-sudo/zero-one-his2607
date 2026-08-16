#pragma once
#ifndef _GET_CATEGORY_DETAILS_QUERY_
#define _GET_CATEGORY_DETAILS_QUERY_

#include "../../GlobalInclude.h" 
#include OATPP_CODEGEN_BEGIN(DTO)

class GetCategoryDetailsQuery : public oatpp::DTO {
	DTO_INIT(GetCategoryDetailsQuery, DTO)
	API_DTO_FIELD(oatpp::String, inpatient_no, ZH_WORDS_GETTER("qi.charging.settle.inpatient_no"), false, "");
	API_DTO_FIELD(oatpp::String, encounter_id, ZH_WORDS_GETTER("qi.charging.settle.encounter_id"), false, "");
};

#include OATPP_CODEGEN_END(DTO)
#endif


