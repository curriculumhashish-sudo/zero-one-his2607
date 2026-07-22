#pragma once

#ifndef _PRESCRIPTION_NEW_PR_DTO_
#define _PRESCRIPTION_NEW_PR_DTO_

#include "../../GlobalInclude.h"
#include "PrescriptionNewMedicineDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个新增处方的数据传输模型，单个处方
* （前端需要传给后端的数据）
*/
class PrescriptionNewPrDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionNewPrDTO, DTO);
	// 处方组key
	API_DTO_FIELD_DEFAULT(String, prescriptionGroupKey, ZH_WORDS_GETTER("prescription.field.prescriptionGroupKey"));
	// 长期或短期
	API_DTO_FIELD_DEFAULT(String, therapyEnum, ZH_WORDS_GETTER("prescription.field.therapyEnum"));
	// 症状组
	API_DTO_FIELD_DEFAULT(String, tcmSyndromeGroup, ZH_WORDS_GETTER("prescription.field.tcmSyndromeGroup"));
	// 用法编码
	API_DTO_FIELD_DEFAULT(String, methodCode, ZH_WORDS_GETTER("prescription.field.methodCode"));
	// 频次编码
	API_DTO_FIELD_DEFAULT(String, rateCode, ZH_WORDS_GETTER("prescription.field.rateCode"));
	// 天数
	API_DTO_FIELD_DEFAULT(Int32, days, ZH_WORDS_GETTER("prescription.field.days"));
	// 付数
	API_DTO_FIELD_DEFAULT(Int32, chineseHerbsDoseQuantity, ZH_WORDS_GETTER("prescription.field.chineseHerbsDoseQuantity"));
	// 是否代煎
	API_DTO_FIELD_DEFAULT(Boolean, sufferingFlag, ZH_WORDS_GETTER("prescription.field.sufferingFlag"));
	// 备注
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("prescription.field.remark"));
	// 药品的集合
	API_DTO_FIELD_DEFAULT(List<PrescriptionNewMedicineDTO::Wrapper>, lines, ZH_WORDS_GETTER("prescription.field.lines"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_NEW_PR_DTO_