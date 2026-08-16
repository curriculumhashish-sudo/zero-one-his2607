#pragma once

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 三个医嘱操作统一返回实际更新的医嘱数量。
class MedicalOrderOperationResultDTO : public oatpp::DTO
{
	DTO_INIT(MedicalOrderOperationResultDTO, DTO);
	API_DTO_FIELD_DEFAULT(Int32, updatedCount, ZH_WORDS_GETTER("advice.field.updatedCount"));
};

#include OATPP_CODEGEN_END(DTO)
