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
	DTO_INIT(MedicalRecordQueryByTitle, PageQuery);
	//病历名称
	DTO_FIELD(String, title);
	DTO_FIELD_INFO(title) {
		info->required = false;
		info->description = ZH_WORDS_GETTER("inpatient-record.title");
	}
};
//指定id查询
class MedicalRecordQueryById :public oatpp::DTO
{
	//定义
	DTO_INIT(MedicalRecordQueryById, DTO);
	//病历id
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->required = true;
		info->description = ZH_WORDS_GETTER("inpatient-record.id");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif



