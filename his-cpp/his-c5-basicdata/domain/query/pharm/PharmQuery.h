#pragma once

#ifndef _PHARM_QUERY_
#define _PHARM_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmQuery : public oatpp::DTO
{
	 // 初始化定义
	DTO_INIT(PharmQuery, DTO);
	// 查询页码
	DTO_FIELD(UInt64, pageIndex) = 1;
	DTO_FIELD_INFO(pageIndex) {
		info->required = true;
#ifndef LINUX
		info->description = u8"当前页码";
#else
		info->description = "curr page index";
#endif
	}
	// 查询条数
	DTO_FIELD(UInt64, pageSize) = 10;
	DTO_FIELD_INFO(pageSize) {
		info->required = true;
#ifndef LINUX
		info->description = u8"每页数据条数";
#else
		info->description = "curr page size";
#endif
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PHARM_QUERY_
