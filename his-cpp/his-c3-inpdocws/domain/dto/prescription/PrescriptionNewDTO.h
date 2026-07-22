#pragma once

#ifndef _PRESCRIPTION_NEW_DTO_
#define _PRESCRIPTION_NEW_DTO_

#include "../../GlobalInclude.h"
#include "PrescriptionNewPrDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个新增处方的数据传输模型
*/
class PrescriptionNewDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionNewDTO, DTO);
	// 挂号id
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("prescription.field.encounterId"));
	// 病人id
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("prescription.field.patientId"));
	// 封装一层prescriptions
	API_DTO_FIELD_DEFAULT(List<PrescriptionNewPrDTO::Wrapper>, prescriptions, ZH_WORDS_GETTER("prescription.field.prescriptions"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_NEW_DTO_