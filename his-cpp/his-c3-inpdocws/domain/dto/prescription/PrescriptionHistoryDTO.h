#pragma once

#ifndef _PRESCRIPTION_HISTORY_DTO_
#define _PRESCRIPTION_HISTORY_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询历史处方的数据传输模型
* （后端需要传给前端的数据）
*/
class PrescriptionHistoryDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionHistoryDTO, DTO);
	// 处方号
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("prescription.field.prescriptionNo"));
	//挂号id
	API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("prescription.field.encounterId"));
	//开处方时间
	API_DTO_FIELD_DEFAULT(String, authoredAt, ZH_WORDS_GETTER("prescription.field.authoredAt"));
	// 药的种类（几味）
	API_DTO_FIELD_DEFAULT(String, summaryText, ZH_WORDS_GETTER("prescription.field.summaryText"));
	// 同一个处方下所有药的名称
	API_DTO_FIELD_DEFAULT(List<String>, medicationNames, ZH_WORDS_GETTER("prescription.field.medicationNames"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_HISTORY_DTO_