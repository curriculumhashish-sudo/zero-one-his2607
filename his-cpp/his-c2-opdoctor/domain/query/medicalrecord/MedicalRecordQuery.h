#ifndef _MEDICALRECORD_QUERY_H_
#define _MEDICALRECORD_QUERY_H_

#include "../../GlobalInclude.h"
#include"domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicalRecordQuery : public PageQuery
{
	DTO_INIT(MedicalRecordQuery, PageQuery);
	//»¼Õß²¡ÀúºÅ
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("medicalrecord.field.patientid"), true);
	//´¦·½ºÅ
	API_DTO_FIELD_REQUIRE(String, presId, ZH_WORDS_GETTER("chinapres.field.presid"), false);
	//ËÑË÷¹Ø¼ü´Ê
	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("medicalrecord.field.keyword1"), false);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_MEDICALRECORD_QUERY_H_