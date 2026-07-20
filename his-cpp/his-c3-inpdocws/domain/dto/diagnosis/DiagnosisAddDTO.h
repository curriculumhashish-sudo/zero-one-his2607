#pragma once
#ifndef _DIAGNOSISADDDTO_H_
#define _DIAGNOSISADDDTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断添加DTO类
   */
class DiagnosisAddDTO : public oatpp::DTO
{
	DTO_INIT(DiagnosisAddDTO, DTO);
	//患者id
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("diagnosis.field.patient_id"), true);
	//就诊id
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("diagnosis.field.encounter_id"), true);
	//是否主诊断
	API_DTO_FIELD_REQUIRE(Int32, primary_flag, ZH_WORDS_GETTER("diagnosis.field.primary_flag"), false);
	//是否设为常用
	API_DTO_FIELD_REQUIRE(Int32, set_always, ZH_WORDS_GETTER("diagnosis.field.set_always"), false);
	//排序
	API_DTO_FIELD_REQUIRE(Int32, rank, ZH_WORDS_GETTER("diagnosis.field.rank"), false);

};





#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSISADDDTO_H_
