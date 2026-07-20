#pragma once
/*
* @Auther:shelly
* @Date:2026/7/20
*/
#ifndef _SUPPLIER_QUERY_
#define _SUPPLIER_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*供应商分页查询参数类
* 继承PageQuery
*/
class SupplierQuery : public PageQuery
{
	DTO_INIT(SupplierQuery, PageQuery);

	//搜索关键字（匹配供应商名称，模糊查询用）
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword) {
		info->description = ZH_WORDS_GETTER("sup.field.keyword");
	}

	//状态过滤(按照状态枚举值筛选，"NORMAL"/"DISABLED");
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif

