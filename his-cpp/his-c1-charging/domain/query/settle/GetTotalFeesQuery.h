#pragma once
#ifndef _GET_TOTAL_FEES_QUERY_
#define _GET_TOTAL_FEES_QUERY_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class GetTotalFeesQuery : public oatpp::DTO
{
    DTO_INIT(GetTotalFeesQuery, DTO);
   
    API_DTO_FIELD(oatpp::String, inpatient_no, ZH_WORDS_GETTER("qi.charging.settle.inpatient_no"), true, "");
    API_DTO_FIELD(oatpp::String, encounter_id, ZH_WORDS_GETTER("qi.charging.settle.encounter_id"), true, "");
   
};

#include OATPP_CODEGEN_END(DTO)
#endif