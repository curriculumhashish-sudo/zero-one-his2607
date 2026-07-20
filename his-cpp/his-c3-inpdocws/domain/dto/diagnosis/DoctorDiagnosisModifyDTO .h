#pragma once
#pragma once
#ifndef _DOCTORDIANGOSISMODIFYDTO_H_
#define _DOCTORDIANGOSISMODIFYDTO_H_
#include "DoctorDiagnosisAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
* 西医诊断修改DTO类
*/
class DoctorDiagnosisModifyDTO : public DoctorDiagnosisAddDTO
{
	DTO_INIT(DoctorDiagnosisModifyDTO, DoctorDiagnosisAddDTO);
	//临床id
	API_DTO_FIELD_REQUIRE(String, condition_id, ZH_WORDS_GETTER("diagnosis.field.condition_id"), true);
	//就诊诊断id
	API_DTO_FIELD_REQUIRE(String, encounter_diagnosis_id, ZH_WORDS_GETTER("diagnosis.field.encounter_diagnosis_id"), true);
	//诊断类型
	API_DTO_FIELD_REQUIRE(String, diag_type_code, ZH_WORDS_GETTER("diagnosis.field.diag_type_code"), false);

};


#include OATPP_CODEGEN_END(DTO)
#endif // !_DOCTORDIANGOSISMODIFYDTO_H_
