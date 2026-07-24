#pragma once

#ifndef _PHARM_QUERY_
#define _PHARM_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmQuery : public oatpp::DTO
{
	DTO_INIT(PharmQuery, DTO);
	// 字段
	// 供应商名称
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pharm.field.name");
	}
	// 类型枚举值 -- 库房"REGISTRY" 药房"DRUG"科室药房""
	DTO_FIELD(String, typeEnum);
	DTO_FIELD_INFO(typeEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.typeEnum");
	}
	// 状态 NORMAL启用 DISTANCE停用
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PHARM_QUERY_
