#pragma once

#ifndef _PRESCRIPTION_CURRENT_QUERY_
#define _PRESCRIPTION_CURRENT_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询用户现有处方的数据传输模型
* （前端需要传给后端的数据）
*/
class PrescriptionCurrentQuery : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionCurrentQuery, DTO);
	// 当前挂号id
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("prescription.field.encounterId"), true);
	// 病人id
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("prescription.field.patientId"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_CURRENT_QUERY_