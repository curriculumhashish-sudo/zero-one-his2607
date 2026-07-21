#pragma once
#ifndef _PATIENTFINISHQUERY_H_
#define _PATIENTFINISHQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientFinishQuery : public oatpp::DTO
{
	DTO_INIT(PatientFinishQuery, DTO);

    
    // ÕÍ’Ô
    API_DTO_FIELD_REQUIRE(String, visitStatus, ZH_WORDS_GETTER("finish.query.visitStatus"), true);

};



#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENTFINISHQUERY_H_
