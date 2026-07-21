#pragma once
#ifndef _PrescriptionQuery_H_
#define _PrescriptionQuery_H_

#include"../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PrescriptionQuery :public oatpp::DTO
{
	DTO_INIT(PrescriptionQuery, DTO);
	//患者病例号
	API_DTO_FIELD_DEFAULT(String, num, ZH_WORDS_GETTER("patient.detail.patientNo"));
	//本次/历史处方
	API_DTO_FIELD_DEFAULT(String, type, ZH_WORDS_GETTER("patient.detail.timetype"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUERY_H_
