#pragma once

#ifndef _PAT_PAGE_QUERY_H_
#define _PAT_PAGE_QUERY_H_

#include "../../GlobalInclude.h"
#include "../../lib-oatpp/include/domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者分页查询条件对象，继承分页父类PageQuery
 * 分页基础参数 + 业务筛选：患者姓名、证件号
 */
class PatPageQuery : public PageQuery
{
	DTO_INIT(PatPageQuery, PageQuery);

	// 患者姓名（模糊查询）
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pat.query.field.name");
	}

	// 身份证/证件号
	DTO_FIELD(String, idCard);
	DTO_FIELD_INFO(idCard) {
		info->description = ZH_WORDS_GETTER("pat.query.field.idCard");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_PAGE_QUERY_H_