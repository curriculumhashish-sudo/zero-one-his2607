#pragma once

#ifndef __SUP_DTO_
#define __SUP_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupDTO : public oatpp::DTO
{
	DTO_INIT(SupDTO, DTO)
	DTO_FIELD(UInt64, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("sup.field.id");
	}

	DTO_FIELD(String, supId);
	DTO_FIELD_INFO(supId) {
		info->description = ZH_WORDS_GETTER("sup.field.supId");
	}

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("sup.field.name");
	}

	// string "product"生产厂商,"supplier"供应商
	DTO_FIELD(String, typeEnum);
	DTO_FIELD_INFO(typeEnum) {
		info->description = ZH_WORDS_GETTER("sup.field.typeEnum");
	}

	DTO_FIELD(String, address);
	DTO_FIELD_INFO(address) {
		info->description = ZH_WORDS_GETTER("sup.field.address");
	}

	DTO_FIELD(String, contact);
	DTO_FIELD_INFO(contact) {
		info->description = ZH_WORDS_GETTER("sup.field.contact");
	}

	// 拼音 
	DTO_FIELD(String, pinyin);
	DTO_FIELD_INFO(pinyin) {
		info->description = ZH_WORDS_GETTER("sup.field.pinyin");
	}

	// 五笔
	DTO_FIELD(String, wubi);
	DTO_FIELD_INFO(wubi) {
		info->description = ZH_WORDS_GETTER("sup.field.wubi");
	}

	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
	}
};

class SupPageDTO : public PageDTO<SupDTO::Wrapper>
{
	DTO_INIT(SupPageDTO, PageDTO<SupDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif 