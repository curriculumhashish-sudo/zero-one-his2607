#pragma once

#ifndef _PRESCRIPTION_CURRENT_MEDICINE_DTO_
#define _PRESCRIPTION_CURRENT_MEDICINE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询现有处方内药品的数据传输模型，单个药品
* （后端需要传给前端的数据）
*/
class PrescriptionCurrentMedicineDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionCurrentMedicineDTO, DTO);
	// request_id
	API_DTO_FIELD_DEFAULT(String, medicationRequestId, ZH_WORDS_GETTER("prescription.field.medicationRequestId"));
	// 药品定义id
	API_DTO_FIELD_DEFAULT(String, medicationDefinitionId, ZH_WORDS_GETTER("prescription.field.medicationDefinitionId"));
	// 药品名称
	API_DTO_FIELD_DEFAULT(String, medicationName, ZH_WORDS_GETTER("prescription.field.medicationName"));
	// 几克每付
	API_DTO_FIELD_DEFAULT(Int32, dose, ZH_WORDS_GETTER("prescription.field.dose"));
	// 几元每g
	API_DTO_FIELD_DEFAULT(Int32, retailUnitPrice, ZH_WORDS_GETTER("prescription.field.retailUnitPrice"));
	// 单价单位
	API_DTO_FIELD_DEFAULT(String, unitName, ZH_WORDS_GETTER("prescription.field.unitName"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_CURRENT_MEDICINE_DTO_
