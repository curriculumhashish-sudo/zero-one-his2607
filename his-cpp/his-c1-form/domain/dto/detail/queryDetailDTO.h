#pragma once

#ifndef _SAMPLE_DTO_
#define _SAMPLE_DTO_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 报表细则获取
class DetailDTO : public PageQuery
{
	DTO_INIT(DetailDTO, PageQuery);

	// 测试代码
	DTO_FIELD(String, register_room);
	DTO_FIELD_INFO(register_room) {
		info->description = 'aaa';
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_DTO_