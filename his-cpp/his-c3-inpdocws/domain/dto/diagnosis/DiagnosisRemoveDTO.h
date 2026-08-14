#pragma once
#ifndef _DIAGNOSISREMOVEDTO_H_
#define _DIAGNOSISREMOVEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
* 诊断删除DTO类
*/
class DiagnosisRemoveDTO : public oatpp::DTO
{
    DTO_INIT(DiagnosisRemoveDTO, DTO);

    // 诊断表id
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnosis.field.id"), true);

    // 病情/诊断条件id
    API_DTO_FIELD_REQUIRE(String, conditionId, ZH_WORDS_GETTER("diagnosis.field.conditionId"), true);

    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSISREMOVEDTO_H_