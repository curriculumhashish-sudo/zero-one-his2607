#pragma once

#ifndef _GET_COST_DETAIL_QUERY_H_
#define _GET_COST_DETAIL_QUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetCostDetailQuery : public oatpp::DTO {
	DTO_INIT(GetCostDetailQuery, DTO);

	// 住院就诊记录主键
	API_DTO_FIELD_REQUIRE(
		String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.encounter_id"), true);

	// 患者主键用于校验住院记录归属
	API_DTO_FIELD_REQUIRE(
		String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.patient_id"), true);

	// 收费明细状态筛选条件
	API_DTO_FIELD(
		String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.cost_status_filter"), false, "ALL");
};

#include OATPP_CODEGEN_END(DTO)

#endif
