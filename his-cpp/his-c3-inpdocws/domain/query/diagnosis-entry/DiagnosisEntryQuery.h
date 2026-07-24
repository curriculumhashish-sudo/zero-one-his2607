#pragma once
#ifndef _DIAGNOSISENTRYQUERY_H_
#define _DIAGNOSISENTRYQUERY_H_
#include "../../GlobalInclude.h"
#include "../../../../lib-oatpp/include/domain/query/PageQuery.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
#include OATPP_CODEGEN_BEGIN(DTO)

//直接查询
class DiagnosisEntryQueryNull :public PageQuery
{
	//定义
	DTO_INIT(DiagnosisEntryQueryNull, DTO);
	//租户id
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("diagnosis-entry.tenant_id"), true);
};

//通过患者id查询
class DiagnosisEntryQueryById :public PageQuery
{
	//定义
	DTO_INIT(DiagnosisEntryQueryById, DTO);
	//租户id
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("diagnosis-entry.tenant_id"), true);
	//患者id
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("diagnosis-entry.patient_id"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif
