#pragma once

#ifndef _CHINAPRESQUERY1_H_
#define _CHINAPRESQUERY1_H_

#include "../../GlobalInclude.h"
#include"domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresQuery1 : public PageQuery
{
	DTO_INIT(ChinapresQuery1,PageQuery);
	//患者病历号
	API_DTO_FIELD_REQUIRE(String,patientId, ZH_WORDS_GETTER("chinapres.field.patientid1"),true);
	//处方号
	API_DTO_FIELD_REQUIRE(String, presId, ZH_WORDS_GETTER("chinapres.field.presid"), false);
	//搜索组套/历史处方关键字
	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("chinapres.field.keyword1"), false);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CHINAPRESQUERY1_H_