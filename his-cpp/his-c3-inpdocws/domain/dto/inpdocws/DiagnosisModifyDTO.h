#pragma once
#pragma once
#ifndef _DIANGOSISMODIFYDTO_H_
#define _DIANGOSISMODIFYDTO_H_
#include "DiagnosisAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
* 诊断修改DTO类
*/
class DiagnosisModifyDTO : public DiagnosisAddDTO
{
	DTO_INIT(DiagnosisModifyDTO, DiagnosisAddDTO);
	//诊断编码
	API_DTO_FIELD_REQUIRE(Int32, diag_code, ZH_WORDS_GETTER("inpdocws.field.diag_code"), true);
	//诊断类型
	API_DTO_FIELD_REQUIRE(String, diag_type_code, ZH_WORDS_GETTER("inpdocws.field.diag_type_code"), true);

};


#include OATPP_CODEGEN_END(DTO)
#endif // !_DIANGOSISMODIFYDTO_H_
