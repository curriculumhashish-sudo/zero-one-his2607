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
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("inpdocws.field.id"), true);
	//是否中医诊断
	API_DTO_FIELD_REQUIRE(Int32, tcm_flag, ZH_WORDS_GETTER("inpdocws.field.tcm_flag"), true);
	//中医疾病名称
	API_DTO_FIELD_REQUIRE(String, tcm_condition, ZH_WORDS_GETTER("inpdocws.field.tcm_condition"), true);
	//中医证候名称
	API_DTO_FIELD_REQUIRE(String, tcm_syndrome, ZH_WORDS_GETTER("inpdocws.field.tcm_syndrome"), true);
	//西医疾病
	API_DTO_FIELD_REQUIRE(String, wm_name, ZH_WORDS_GETTER("inpdocws.field.wm_name"), true);
	//是否主诊断
	API_DTO_FIELD_REQUIRE(Int32, primary_flag, ZH_WORDS_GETTER("inpdocws.field.primary_flag"), true);
	//是否设为常用
	API_DTO_FIELD_REQUIRE(Int32, set_always, ZH_WORDS_GETTER("inpdocws.field.set_always"), true);
	
	
};





#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSISADDDTO_H_
