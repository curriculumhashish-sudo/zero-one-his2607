#pragma once

#ifndef _PRESCRIPTION_DELETE_DTO_
#define _PRESCRIPTION_DELETE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个删除处方的请求数据传输模型
*/
class PrescriptionDeleteDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionDeleteDTO, DTO);
	// 请求id列表
	API_DTO_FIELD_DEFAULT(List<String>, id, ZH_WORDS_GETTER("prescription.field.medicationRequestIds"));
	// 挂号id
	API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("prescription.field.encounterId"));
	// 病人id
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("prescription.field.patientId"));
	// 行为
	API_DTO_FIELD_DEFAULT(String, action, ZH_WORDS_GETTER("prescription.field.action"));
	// 领域
	API_DTO_FIELD_DEFAULT(String, scope, ZH_WORDS_GETTER("prescription.field.scope"));
	// 处方组key
	API_DTO_FIELD_DEFAULT(String, prescriptionGroupKey, ZH_WORDS_GETTER("prescription.field.prescriptionGroupKey"));
	// 处方组号
	API_DTO_FIELD_DEFAULT(String, prescriptionNo, ZH_WORDS_GETTER("prescription.field.prescriptionNo"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_DELETE_DTO_