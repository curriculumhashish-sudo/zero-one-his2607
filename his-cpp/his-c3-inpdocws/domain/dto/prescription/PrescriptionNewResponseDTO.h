#pragma once

#ifndef _PRESCRIPTION_NEW_RESPONSE_DTO_
#define _PRESCRIPTION_NEW_RESPONSE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个新增处方响应的数据传输模型
*/
class PrescriptionNewResponseDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionNewResponseDTO, DTO);
	// 成功
	API_DTO_FIELD_DEFAULT(Boolean, success, ZH_WORDS_GETTER("prescription.field.success"));
	// 新request的id
	API_DTO_FIELD_DEFAULT(List<String>, newMedicationRequestIds, ZH_WORDS_GETTER("prescription.field.newMedicationRequestIds"));
	// 信息
	API_DTO_FIELD_DEFAULT(String, message, ZH_WORDS_GETTER("prescription.field.message"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_NEW_RESPONSE_DTO_