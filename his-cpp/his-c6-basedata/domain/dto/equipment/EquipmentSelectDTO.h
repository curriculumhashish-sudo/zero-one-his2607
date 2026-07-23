#pragma once

#ifndef _EQUIPMENTSELECTDTO_H_
#define _EQUIPMENTSELECTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentSelectDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentSelectDTO, DTO);

	// 器材ID
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("equipment.field.id"));
	// 器材名称
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("equipment.field.name"));
	// 器材编码
	API_DTO_FIELD_DEFAULT(String, code, ZH_WORDS_GETTER("equipment.field.code"));
	// 规格
	API_DTO_FIELD_DEFAULT(String, specification, ZH_WORDS_GETTER("equipment.field.specification"));
	// 生产厂家
	API_DTO_FIELD_DEFAULT(String, manufacturer, ZH_WORDS_GETTER("equipment.field.manufacturer"));
};

class EquipmentSelectPageDTO : public PageDTO<EquipmentSelectDTO::Wrapper>
{
	DTO_INIT(EquipmentSelectPageDTO, PageDTO<EquipmentSelectDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif 