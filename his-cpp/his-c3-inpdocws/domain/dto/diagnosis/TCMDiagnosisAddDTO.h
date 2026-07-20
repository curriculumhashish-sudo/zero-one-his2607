#pragma once
#ifndef _TCMDIAGNOSISADDDTO_H_
#define _TCMDIAGNOSISADDDTO_H_
#include "DiagnosisAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 中医诊断添加DTO类
 * 病症和证候共享证候组，由后端生成
   */
class TCMDiagnosisAddDTO : public DiagnosisAddDTO
{
	DTO_INIT(TCMDiagnosisAddDTO, DiagnosisAddDTO);
	

	//疾病id
	API_DTO_FIELD_REQUIRE(String, disease_def_id, ZH_WORDS_GETTER("diagnosis.field.disease_def_id"), true);
	//证候id
	API_DTO_FIELD_REQUIRE(String, syndrome_def_id, ZH_WORDS_GETTER("diagnosis.field.syndrome_def_id"), true);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, diagnosis_desc, ZH_WORDS_GETTER("diagnosis.field.diagnosis_desc"), true);
	

};





#include OATPP_CODEGEN_END(DTO)
#endif // !_TCMDIAGNOSISADDDTO_H_
