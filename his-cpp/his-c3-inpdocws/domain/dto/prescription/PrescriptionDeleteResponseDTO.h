#pragma once

#ifndef _PRESCRIPTION_DELETE_RESPONSE_DTO_
#define _PRESCRIPTION_DELETE_RESPONSE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个删除处方响应的数据传输模型
*/
class PrescriptionDeleteResponseDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionDeleteResponseDTO, DTO);
	// 成功
	API_DTO_FIELD_DEFAULT(Boolean, success, ZH_WORDS_GETTER("prescription.field.success"));
	// 信息
	API_DTO_FIELD_DEFAULT(String, message, ZH_WORDS_GETTER("prescription.field.message"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_DELETE_RESPONSE_DTO_