#pragma once


#ifndef QUERY_DETAIL_QUERY
#define QUERY_DETAIL_QUERY

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


// 获取明细列表的query
class QueryDatail :public PageQuery
{
	DTO_INIT(QueryDatail, PageQuery);

	// 挂号科室
	DTO_FIELD(String, register_room);
	DTO_FIELD_INFO(register_room) {
		info->description = ZH_WORDS_GETTER("rhqm.form.detail.attain.register_room");
	}

	// 患者姓名
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("rhqm.form.detail.attain.name");
	}
};

#include OATPP_CODEGEN_END(DTO)