#pragma once

#ifndef _EQUIPMENTVO_H_
#define _EQUIPMENTVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/equipment/EquipmentDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentDetailPageJsonVO : public JsonVO<EquipmentDetailPageDTO::Wrapper>
{
	DTO_INIT(EquipmentDetailPageJsonVO, JsonVO<EquipmentDetailPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTVO_H_