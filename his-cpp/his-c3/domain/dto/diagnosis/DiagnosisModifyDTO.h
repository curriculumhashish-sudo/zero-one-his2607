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
	API_DTO_FIELD_REQUIRE(String, diagTypeCode, ZH_WORDS_GETTER("diagnosis.field.diagTypeCode"), false);
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
	
};


#include OATPP_CODEGEN_END(DTO)
#endif // !_DIANGOSISMODIFYDTO_H_
