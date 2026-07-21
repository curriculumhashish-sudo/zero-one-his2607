#pragma once

#ifndef _EQUIPMENTDETAILDTO_H_
#define _EQUIPMENTDETAILDTO_H_

#include "EquipmentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 器材详情all DTO
 */
class EquipmentDetailDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentDetailDTO, DTO);
	// 器材ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("equipment.field.id"), true);
	// 器材编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("equipment.field.code"), false);
	// 器材名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("equipment.field.name"), true);
	// 器材分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), true);
	// 规格
	API_DTO_FIELD_REQUIRE(String, specification, ZH_WORDS_GETTER("equipment.field.specification"), true);
	// 状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("equipment.field.status"), true);
	// 生产厂家
	API_DTO_FIELD_REQUIRE(String, manufacturer, ZH_WORDS_GETTER("equipment.field.manufacturer"), false);
	// 价格
	API_DTO_FIELD_REQUIRE(String, price, ZH_WORDS_GETTER("equipment.field.price"), true);
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