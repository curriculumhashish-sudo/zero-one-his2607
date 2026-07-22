#pragma once

#ifndef _PRESCRIPTION_NEW_MEDICINE_DTO_
#define _PRESCRIPTION_NEW_MEDICINE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个新增处方内药品的数据传输模型，单个药品
* （前端需要传给后端的数据）
*/
class PrescriptionNewMedicineDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionNewMedicineDTO, DTO);
	// lineOp
	API_DTO_FIELD_DEFAULT(String, lineOp, ZH_WORDS_GETTER("prescription.field.lineOp"));
	// 药品定义id
	API_DTO_FIELD_DEFAULT(String, medicationDefinitionId, ZH_WORDS_GETTER("prescription.field.medicationDefinitionId"));
	// 几克每付
	API_DTO_FIELD_DEFAULT(Int32, dose, ZH_WORDS_GETTER("prescription.field.dose"));
	// doseUnitCode
	API_DTO_FIELD_DEFAULT(String, doseUnitCode, ZH_WORDS_GETTER("prescription.field.doseUnitCode"));
	// sortNumber
	API_DTO_FIELD_DEFAULT(Int32, sortNumber, ZH_WORDS_GETTER("prescription.field.sortNumber"));
	// dispenseLocationId
	API_DTO_FIELD_DEFAULT(String, dispenseLocationId, ZH_WORDS_GETTER("prescription.field.dispenseLocationId"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_NEW_MEDICINE_DTO_
