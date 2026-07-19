#pragma once

#ifndef _RENTDETAILDTO_H_
#define _RENTDETAILDTO_H_

#include "EquipmentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 器材详情DTO
 */
class EquipmentDetailDTO : public EquipmentAddDTO
{
	DTO_INIT(EquipmentDetailDTO, EquipmentAddDTO);
	// 租户ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("equipment.field.id"), true);
	// 创建人
	API_DTO_FIELD_REQUIRE(String, createBy, ZH_WORDS_GETTER("equipment.field.createBy"), true);
	// 更新人
	API_DTO_FIELD_REQUIRE(String, updateBy, ZH_WORDS_GETTER("equipment.field.updateBy"), true);
	// 其他字段自己补充完整
};

/**
 * 定义一个器材信息分页传输对象
 */
class EquipmentDetailPageDTO : public PageDTO<EquipmentDetailDTO::Wrapper>
{
	DTO_INIT(EquipmentDetailPageDTO, PageDTO<EquipmentDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_RENTDETAILDTO_H_