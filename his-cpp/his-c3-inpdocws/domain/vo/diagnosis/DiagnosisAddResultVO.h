#pragma once
#ifndef _DIAGNOSIS_ADD_RESULT_VO_H_
#define _DIAGNOSIS_ADD_RESULT_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/diagnosis/DiagnosisAddResultDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断新增结果响应VO
 */
class DiagnosisAddResultVO : public JsonVO<DiagnosisAddResultDTO::Wrapper> {
    DTO_INIT(DiagnosisAddResultVO, JsonVO<DiagnosisAddResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSIS_ADD_RESULT_VO_H_