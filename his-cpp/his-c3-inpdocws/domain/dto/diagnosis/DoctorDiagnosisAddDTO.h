#pragma once
#ifndef _DOCTORDIAGNOSISADDDTO_H_
#define _DOCTORDIAGNOSISADDDTO_H_
#include "DiagnosisAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 西医诊断添加DTO类
   */
class DoctorDiagnosisAddDTO : public DiagnosisAddDTO
{
	DTO_INIT(DoctorDiagnosisAddDTO, DiagnosisAddDTO);

	//诊断定义id
	API_DTO_FIELD_REQUIRE(String, condition_def_id, ZH_WORDS_GETTER("diagnosis.field.condition_def_id"), true);
	//病情编码(如CDF00000003)
	API_DTO_FIELD_REQUIRE(Int32, condition_code, ZH_WORDS_GETTER("diagnosis.field.condition_code"), true);
	//疾病名称
	API_DTO_FIELD_REQUIRE(String, condition_name, ZH_WORDS_GETTER("diagnosis.field.condition_name"), true);


};


#include OATPP_CODEGEN_END(DTO)
#endif // !_DOCTORDIAGNOSISADDDTO_H_
