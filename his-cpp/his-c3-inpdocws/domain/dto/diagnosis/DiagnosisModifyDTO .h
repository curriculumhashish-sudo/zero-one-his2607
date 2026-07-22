#pragma once
#pragma once
#ifndef _DIANGOSISMODIFYDTO_H_
#define _DIANGOSISMODIFYDTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
* 诊断修改DTO类
*/
class DiagnosisModifyDTO : public oatpp::DTO
{
	DTO_INIT(DiagnosisModifyDTO, DTO);
	//诊断表id
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnosis.field.id"), true);
	//诊断类型
	API_DTO_FIELD_REQUIRE(String, diag_type_code, ZH_WORDS_GETTER("diagnosis.field.diag_type_code"), false);

};


#include OATPP_CODEGEN_END(DTO)
#endif // !_DIANGOSISMODIFYDTO_H_
