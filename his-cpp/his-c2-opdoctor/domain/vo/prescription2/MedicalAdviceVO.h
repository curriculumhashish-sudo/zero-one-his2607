#pragma once

#ifndef _MEDICAL_ADVICE_VO_
#define _MEDICAL_ADVICE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription2/PrintApplicationFormDTO.h"
#include "../../dto/prescription2/SignMedicalAdviceDTO.h"
#include "../../dto/prescription2/WithdrawMedicalAdviceDTO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// 包含已成功签发医嘱标识的响应。
class SignMedicalAdviceJsonVO : public JsonVO<List<String>>
{
    DTO_INIT(SignMedicalAdviceJsonVO, JsonVO<List<String>>);
};

// 包含已成功撤回医嘱标识的响应。
class WithdrawMedicalAdviceJsonVO : public JsonVO<List<String>>
{
    DTO_INIT(WithdrawMedicalAdviceJsonVO, JsonVO<List<String>>);
};

// 包含已打印服务申请单标识的响应。
class PrintApplicationFormJsonVO : public JsonVO<String>
{
    DTO_INIT(PrintApplicationFormJsonVO, JsonVO<String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ADVICE_VO_
