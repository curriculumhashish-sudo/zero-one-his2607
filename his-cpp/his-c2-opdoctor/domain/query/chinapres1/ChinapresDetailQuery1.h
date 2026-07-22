#pragma once

#ifndef _CHINAPRESDETAILQUERY1_H_
#define _CHINAPRESDETAILQUERY1_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresDetailQuery1 : public  oatpp::DTO
{
	DTO_INIT(ChinapresDetailQuery1, DTO);
	//患者病历号
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("chinapres.field.patientid"), true);
	//处方号
	API_DTO_FIELD_REQUIRE(String, presId, ZH_WORDS_GETTER("chinapres.field.presid"), false);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CHINAPRESDETAILQUERY1_H_