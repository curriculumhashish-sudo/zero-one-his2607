#pragma once

#ifndef _EQUIPMENTADDDTO_H_
#define _EQUIPMENTADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 租户添加DTO类
 */
class EquipmentAddDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentAddDTO, DTO);
	
	// 器材名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), true);
	// 器材分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTADDDTO_H_