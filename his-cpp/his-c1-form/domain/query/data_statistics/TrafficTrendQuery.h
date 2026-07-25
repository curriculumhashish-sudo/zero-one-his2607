#pragma once

#ifndef _TRAFFIC_TREND_QUERY_
#define _TRAFFIC_TREND_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 流量趋势查询对象
 */
class TrafficTrendQuery : public PageQuery
{
	DTO_INIT(TrafficTrendQuery, PageQuery);

	// 时间维度（1=周，2=月）
	DTO_FIELD(UInt32, timeType);
	DTO_FIELD_INFO(timeType) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.time_type");
	}

	// 开始日期
	DTO_FIELD(String, startDate);
	DTO_FIELD_INFO(startDate) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.start_date");
	}

	// 结束日期
	DTO_FIELD(String, endDate);
	DTO_FIELD_INFO(endDate) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.end_date");
	}

	// 机构ID
	DTO_FIELD(UInt32, orgId);
	DTO_FIELD_INFO(orgId) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.org_id");
	}

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TRAFFIC_TREND_QUERY_