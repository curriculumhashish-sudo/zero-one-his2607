#pragma once

#ifndef _PHARM_DTO_
#define _PHARM_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmDTO : public oatpp::DTO
{
	// 初始化定义
	DTO_INIT(PharmDTO, DTO);

	// 字段
	// 供应商ID
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("pharm.field.id");
	}

	// 供应商名称
	DTO_FIELD(String, name)
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pharm.field.name");
	}

	// 类型枚举值 -- 库房"REGISTRY" 药房"DRUG"科室药房""
	DTO_FIELD(String, typeEnum);
	DTO_FIELD_INFO(typeEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.typeEnum");
	}

	// 上级科室 classRoom
	DTO_FIELD(String, classRoom);
	DTO_FIELD_INFO(classRoom) {
		info->description = ZH_WORDS_GETTER("pharm.field.classRoom");
	}

	// 状态 NORMAL启用 DISTANCE停用
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
	}

};

class PharmPageDTO : public PageDTO<PharmDTO::Wrapper>
{
	DTO_INIT(PharmPageDTO, PageDTO<PharmDTO::Wrapper>);
};

class PharmBatchEnableDisableDTO : public oatpp::DTO {
	DTO_INIT(PharmBatchEnableDisableDTO, DTO);

	DTO_FIELD(List<String>, ids);
	DTO_FIELD_INFO(ids) {
		info->description = ZH_WORDS_GETTER("pharm.field.ids");
	}

	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("pharm.field.statusEnum");
	}
};



#include OATPP_CODEGEN_END(DTO)

#endif	// !_PHARM_DTO_