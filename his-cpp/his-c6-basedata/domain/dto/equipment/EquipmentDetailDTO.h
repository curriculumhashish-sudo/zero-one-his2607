#pragma once

#ifndef _EQUIPMENTDETAILDTO_H_
#define _EQUIPMENTDETAILDTO_H_

#include "EquipmentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 器材详情DTO
 */
class EquipmentDetailDTO : public EquipmentAddDTO
{
	DTO_INIT(EquipmentDetailDTO, EquipmentAddDTO);
	// 器材ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("equipment.field.id"), true);
	// 器材名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), true);
	// 器材分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), true);
};

/**
 * 定义一个器材信息分页传输对象
 */
class EquipmentDetailPageDTO : public PageDTO<EquipmentDetailDTO::Wrapper>
{
	DTO_INIT(EquipmentDetailPageDTO, PageDTO<EquipmentDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTDETAILDTO_H_