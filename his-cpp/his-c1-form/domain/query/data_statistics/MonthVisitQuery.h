#pragma once

#ifndef _MONTH_VISIT_QUERY_
#define _MONTH_VISIT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class MonthVisitQuery : public PageQuery
{
	DTO_INIT(MonthVisitQuery, PageQuery);

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

#endif // !_MONTH_VISIT_QUERY_