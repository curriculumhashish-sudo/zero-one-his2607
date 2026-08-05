#pragma once

#ifndef _MEDICAL_ADVICE_VO_
#define _MEDICAL_ADVICE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription2/PrintApplicationFormDTO.h"
#include "../../dto/prescription2/SignMedicalAdviceDTO.h"
#include "../../dto/prescription2/WithdrawMedicalAdviceDTO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// JSON response returned after medical advice signing succeeds.
class SignMedicalAdviceJsonVO : public JsonVO<List<String>>
{
    DTO_INIT(SignMedicalAdviceJsonVO, JsonVO<List<String>>);
};

// JSON response returned after medical advice withdrawal succeeds.
class WithdrawMedicalAdviceJsonVO : public JsonVO<List<String>>
{
    DTO_INIT(WithdrawMedicalAdviceJsonVO, JsonVO<List<String>>);
};

// JSON response returned after service request printing succeeds.
class PrintApplicationFormJsonVO : public JsonVO<String>
{
    DTO_INIT(PrintApplicationFormJsonVO, JsonVO<String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ADVICE_VO_
