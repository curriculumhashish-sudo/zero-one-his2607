#pragma once

#ifndef _EQUIPMENTSELECTDTO_H_
#define _EQUIPMENTSELECTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentSelectDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentSelectDTO, DTO);

	// ん第ID
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("equipment.field.id"));
	// ん第晤鎢
	API_DTO_FIELD_DEFAULT(String, code, ZH_WORDS_GETTER("equipment.field.code"));
	// ん第靡備
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("equipment.field.name"));
	// 等弇
	API_DTO_FIELD_DEFAULT(String, unit, ZH_WORDS_GETTER("equipment.field.unit"));
};

class EquipmentSelectPageDTO : public PageDTO<EquipmentSelectDTO::Wrapper>
{
	DTO_INIT(EquipmentSelectPageDTO, PageDTO<EquipmentSelectDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif 