#pragma once
#ifndef _MEDICALRECORDQUERY_H_
#define _MEDICALRECORDQUERY_H_
#include "../../GlobalInclude.h"
#include "../../../../lib-oatpp/include/domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
//通过名称查询
class MedicalRecordQueryByTitle :public PageQuery
{
	//定义
	DTO_INIT(MedicalRecordQueryByTitle, DTO);
	//病历名称
	API_DTO_FIELD_REQUIRE(String, title, ZH_WORDS_GETTER("inpatient-record.title"), false);
};
//指定id查询
class MedicalRecordQueryById :public oatpp::DTO
{
	//定义
	DTO_INIT(MedicalRecordQueryById, DTO);
	//病历id
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("inpatient-record.id"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif
