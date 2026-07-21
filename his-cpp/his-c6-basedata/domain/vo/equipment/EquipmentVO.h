#pragma once

#ifndef _EQUIPMENTVO_H_
#define _EQUIPMENTVO_H_

#include "../../dto/equipment/EquipmentDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 定义一个器材信息分页显示JsonVO对象，用于响应给客户端
 */
class EquipmentDetailPageJsonVO : public JsonVO<EquipmentDetailPageDTO::Wrapper>
{
	DTO_INIT(EquipmentDetailPageJsonVO, JsonVO<EquipmentDetailPageDTO::Wrapper>);
};

/**
 * 响应给客户端的用户详细信息JsonVO对象
 */
class EquipmentDetailJsonVO : public JsonVO<EquipmentAddDTO::Wrapper>
{
	DTO_INIT(EquipmentDetailJsonVO, JsonVO<EquipmentAddDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTVO_H_