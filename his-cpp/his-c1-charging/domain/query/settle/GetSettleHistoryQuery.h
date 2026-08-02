#pragma once

#ifndef _GET_SETTLE_HISTORY_QUERY_H_
#define _GET_SETTLE_HISTORY_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSettleHistoryQuery : public PageQuery {
	DTO_INIT(GetSettleHistoryQuery, PageQuery);

	// 患者主键
	API_DTO_FIELD_REQUIRE(
		String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.patient_id"), true);

	// 可选的住院就诊记录主键
	API_DTO_FIELD(
		String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.history_encounter_id"), false, "");

	// 结算状态筛选条件
	API_DTO_FIELD(
		String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.history_status_filter"), false, "ALL");
};

#include OATPP_CODEGEN_END(DTO)

#endif
