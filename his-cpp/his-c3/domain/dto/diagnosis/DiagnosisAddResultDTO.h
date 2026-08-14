#pragma once
#ifndef _DIAGNOSIS_ADD_RESULT_DTO_H_
#define _DIAGNOSIS_ADD_RESULT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断新增结果DTO类
 */
class DiagnosisAddResultDTO : public oatpp::DTO
{
    DTO_INIT(DiagnosisAddResultDTO, DTO);

    DTO_FIELD(String, diagnosisId);
    DTO_FIELD(String, conditionId);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSIS_ADD_RESULT_DTO_H_