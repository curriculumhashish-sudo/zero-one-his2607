#pragma once
#ifndef _PAT_SELECT_PAGE_QUERY_H_
#define _PAT_SELECT_PAGE_QUERY_H_

#include "../../GlobalInclude.h"
#include "../../lib-oatpp/include/domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 选择患者弹窗分页查询条件
 */
class PatSelectPageQuery : public PageQuery
{
	DTO_INIT(PatSelectPageQuery, PageQuery);

	// 搜索关键词：姓名 / 病历号 / 证件号 模糊查询
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword) {
		info->description = ZH_WORDS_GETTER("pat.select.query.field.keyword");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_SELECT_PAGE_QUERY_H_