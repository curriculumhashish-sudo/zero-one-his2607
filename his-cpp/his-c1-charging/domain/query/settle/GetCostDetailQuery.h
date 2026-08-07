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

	// 显示模式，SUMMARY仅返回汇总，DETAIL返回汇总和费用明细
	API_DTO_FIELD(
		String, presentationMode, ZH_WORDS_GETTER("ovo.charging.settle.cost_presentation_mode"), false, "DETAIL");

	// 费用开始日期，格式yyyy-MM-dd或yyyy-MM-dd HH:mm:ss
	API_DTO_FIELD(
		String, chargeDateFrom, ZH_WORDS_GETTER("ovo.charging.settle.charge_date_from"), false, "");

	// 费用结束日期，格式yyyy-MM-dd或yyyy-MM-dd HH:mm:ss
	API_DTO_FIELD(
		String, chargeDateTo, ZH_WORDS_GETTER("ovo.charging.settle.charge_date_to"), false, "");
};

#include OATPP_CODEGEN_END(DTO)

#endif
