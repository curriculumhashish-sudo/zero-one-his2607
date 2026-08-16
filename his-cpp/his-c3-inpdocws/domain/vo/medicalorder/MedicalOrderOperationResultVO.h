#pragma once

#include "../../GlobalInclude.h"
#include "../../dto/medicalorder/MedicalOrderOperationResultDTO.h"
#include "../../../../lib-oatpp/include/domain/vo/JsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 统一使用标准JSON响应包装业务结果，失败时data为空。
class MedicalOrderOperationResultVO : public JsonVO<MedicalOrderOperationResultDTO::Wrapper>
{
	DTO_INIT(MedicalOrderOperationResultVO, JsonVO<MedicalOrderOperationResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
