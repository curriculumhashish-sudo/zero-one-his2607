#pragma once

#ifndef _EQUIPMENTVO_H_
#define _EQUIPMENTVO_H_

#include "../../dto/equipment/EquipmentDetailDTO.h"
#include "../../dto/equipment/EquipmentSelectDTO.h"
#include "../../dto/equipment/EquipmentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 定义一个器材信息分页显示JsonVO对象，用于响应给客户端
 */
class EquipmentPageJsonVO : public JsonVO<EquipmentDetailPageDTO::Wrapper>
{
	DTO_INIT(EquipmentPageJsonVO, JsonVO<EquipmentDetailPageDTO::Wrapper>);
};

/**
 * 示例显示JsonVO，用于响应给客户端的Json对象
 */
class EquipmentJsonVO : public JsonVO<EquipmentDTO::Wrapper>
{
	DTO_INIT(EquipmentJsonVO, JsonVO<EquipmentDTO::Wrapper>);
};

/**
 * 选择器材下拉列表的分页响应视图模型
 */
class EquipmentSelectPageJsonVO : public JsonVO<EquipmentSelectPageDTO::Wrapper> {
	DTO_INIT(EquipmentSelectPageJsonVO, JsonVO<EquipmentSelectPageDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTVO_H_