#pragma once

#ifndef _PRESCRIPTION_CURRENT_DTO_
#define _PRESCRIPTION_CURRENT_DTO_

#include "../../GlobalInclude.h"
#include "PrescriptionCurrentMedicineDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询现有处方的数据传输模型，单个处方
* （后端需要传给前端的数据）
*/
class PrescriptionCurrentDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionCurrentDTO, DTO);
	// 处方号
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("prescription.field.prescriptionNo"));
	// 处方状态
	API_DTO_FIELD_DEFAULT(String, statusEnum, ZH_WORDS_GETTER("prescription.field.statusEnum"));
	// 长期或短期
	API_DTO_FIELD_DEFAULT(String, therapyEnum, ZH_WORDS_GETTER("prescription.field.therapyEnum"));
	// 药的用法
	API_DTO_FIELD_DEFAULT(String, methodName, ZH_WORDS_GETTER("prescription.field.methodName"));
	// 频次
	API_DTO_FIELD_DEFAULT(String, rateName, ZH_WORDS_GETTER("prescription.field.rateName"));
	// 天数
	API_DTO_FIELD_DEFAULT(Int32, days, ZH_WORDS_GETTER("prescription.field.days"));
	// 付数
	API_DTO_FIELD_DEFAULT(Int32, chineseHerbsDoseQuantity, ZH_WORDS_GETTER("prescription.field.chineseHerbsDoseQuantity"));
	// 是否代煎
	API_DTO_FIELD_DEFAULT(Boolean, sufferingFlag, ZH_WORDS_GETTER("prescription.field.sufferingFlag"));
	// 诊断
	API_DTO_FIELD_DEFAULT(String, description, ZH_WORDS_GETTER("prescription.field.description"));
	// 备注
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("prescription.field.remark"));
	// 药品的集合
	API_DTO_FIELD_DEFAULT(List<PrescriptionCurrentMedicineDTO::Wrapper>, lines, ZH_WORDS_GETTER("prescription.field.lines"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_CURRENT_DTO_