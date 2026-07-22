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
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("diagnosis.field.tenant_id"), true);
	//就诊id
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("diagnosis.field.encounter_id"), true);
	//是否主诊断
	API_DTO_FIELD_REQUIRE(Int32, primary_flag, ZH_WORDS_GETTER("diagnosis.field.primary_flag"), false);
	//是否设为常用
	API_DTO_FIELD_REQUIRE(Int32, set_always, ZH_WORDS_GETTER("diagnosis.field.set_always"), false);
	//排序
	API_DTO_FIELD_REQUIRE(Int32, rank, ZH_WORDS_GETTER("diagnosis.field.rank"), false);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("diagnosis.field.description"), true);
	//病情编码(如CDF00000003)
	API_DTO_FIELD_REQUIRE(Int32, condition_code, ZH_WORDS_GETTER("diagnosis.field.condition_code"), false);
	//是否中医
	API_DTO_FIELD_REQUIRE(Int32, tcm_flag, ZH_WORDS_GETTER("diagnosis.field.tcm_flag"), true);
	//疾病id
	//API_DTO_FIELD_REQUIRE(String, disease_id, ZH_WORDS_GETTER("diagnosis.field.disease_def_id"), true);
	//证候id
	//API_DTO_FIELD_REQUIRE(String, syndrome_id, ZH_WORDS_GETTER("diagnosis.field.syndrome_def_id"), true);
	//病情/诊断条件id
	//API_DTO_FIELD_REQUIRE(String, condition_id, ZH_WORDS_GETTER("diagnosis.field.condition_id"), true);
	
	//CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	
};





#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSISADDDTO_H_
