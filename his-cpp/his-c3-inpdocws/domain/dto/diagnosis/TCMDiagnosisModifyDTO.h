#pragma once
#pragma once
#ifndef _TCMDIANGOSISMODIFYDTO_H_
#define _TCMDIANGOSISMODIFYDTO_H_
#include "DiagnosisAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
* 中医诊断修改DTO类
*/
class TCMDiagnosisModifyDTO : public DiagnosisAddDTO
{
	DTO_INIT(TCMDiagnosisModifyDTO, DiagnosisAddDTO);
	
	//病情编码(如CDF00000003)
	API_DTO_FIELD_REQUIRE(Int32, condition_code, ZH_WORDS_GETTER("diagnosis.field.condition_code"), true);
	//诊断定义id
	API_DTO_FIELD_REQUIRE(String, condition_def_id, ZH_WORDS_GETTER("diagnosis.field.condition_def_id"), true);
	//临床id
	API_DTO_FIELD_REQUIRE(String, condition_id, ZH_WORDS_GETTER("diagnosis.field.condition_id"), true);
	//疾病名称
	API_DTO_FIELD_REQUIRE(String, condition_name, ZH_WORDS_GETTER("diagnosis.field.condition_name"), true);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, diagnosis_desc, ZH_WORDS_GETTER("diagnosis.field.diagnosis_desc"), true);
	//就诊诊断id
	API_DTO_FIELD_REQUIRE(String, encounter_diagnosis_id, ZH_WORDS_GETTER("diagnosis.field.encounter_diagnosis_id"), true);
	//诊断类型
	API_DTO_FIELD_REQUIRE(String, diag_type_code, ZH_WORDS_GETTER("diagnosis.field.diag_type_code"), true);
	//是否中医
	API_DTO_FIELD_REQUIRE(Int32, tcm_flag, ZH_WORDS_GETTER("diagnosis.field.tcm_flag"), true);

};


#include OATPP_CODEGEN_END(DTO)
#endif // !_TCMDIANGOSISMODIFYDTO_H_
