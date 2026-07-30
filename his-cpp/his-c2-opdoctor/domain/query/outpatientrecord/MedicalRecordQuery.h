#ifndef _MEDICALRECORD_QUERY_H_
#define _MEDICALRECORD_QUERY_H_

#include "../../GlobalInclude.h"
#include"domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicalRecordQuery : public PageQuery
{
	DTO_INIT(MedicalRecordQuery, PageQuery);
	//患者病历号
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("medicalrecord.field.patientid"), true);
	//就诊号
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("medicalrecord.field.encounterid"), false);
	//搜索病历名称
	API_DTO_FIELD_REQUIRE(String, recordName, ZH_WORDS_GETTER("medicalrecord.field.recordname"), false);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_MEDICALRECORD_QUERY_H_