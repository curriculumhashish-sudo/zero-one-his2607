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

	// 可选的支付流水主键，用于定位单条结算记录
	API_DTO_FIELD(
		String, paymentId, ZH_WORDS_GETTER("ovo.charging.settle.payment_id"), false, "");

	// 显示模式，SUMMARY仅返回汇总，DETAIL返回汇总、支付记录和费用明细
	API_DTO_FIELD(
		String, presentationMode, ZH_WORDS_GETTER("ovo.charging.settle.history_presentation_mode"), false, "DETAIL");
};

#include OATPP_CODEGEN_END(DTO)

#endif
