#pragma once

#ifndef _PRESCRIPTION_CURRENT_LIST_DTO_
#define _PRESCRIPTION_CURRENT_LIST_DTO_

#include "../../GlobalInclude.h"
#include "PrescriptionCurrentDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询现有处方的List数据传输模型
* 再封装一层的原因是为了增加prescriptions字段
*/
class PrescriptionCurrentListDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionCurrentListDTO, DTO);
	// 封装一层prescriptions
	API_DTO_FIELD_DEFAULT(List<PrescriptionCurrentDTO::Wrapper>, prescriptions, ZH_WORDS_GETTER("prescription.field.prescriptions"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_CURRENT_LIST_DTO_