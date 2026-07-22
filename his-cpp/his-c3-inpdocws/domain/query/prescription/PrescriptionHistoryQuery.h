#pragma once

#ifndef _PRESCRIPTION_HISTORY_QUERY_
#define _PRESCRIPTION_HISTORY_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询用户历史处方的数据传输模型
* （前端需要传给后端的数据）
*/
class PrescriptionHistoryQuery : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionHistoryQuery, DTO);
	// 当前挂号id
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("prescription.field.encounterId"),true);
	// 病人id
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("prescription.field.patientId"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_HISTORY_QUERY_