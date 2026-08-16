#pragma once
#ifndef _PAT_SELECT_PAGE_QUERY_H_
#define _PAT_SELECT_PAGE_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Query params for select-patient popup (name / medical no / id card).
 */
class PatSelectPageQuery : public PageQuery
{
	DTO_INIT(PatSelectPageQuery, PageQuery);

	// Keyword: name / bus_no / id_card fuzzy match
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword) {
		info->description = ZH_WORDS_GETTER("pat.select.query.field.keyword");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_SELECT_PAGE_QUERY_H_