#pragma once
#ifndef _INVENTORYMATERIALVO_H_
#define _INVENTORYMATERIALVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/data_statistics/InventoryMaterialDto.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// œ‘ æJsonVO
class InventoryMaterialVo : public JsonVO<InventoryMaterialDto::Wrapper> {
	DTO_INIT(InventoryMaterialVo, JsonVO<InventoryMaterialDto::Wrapper>);
};

// ∑÷“≥œ‘ æJsonVO
class InventoryMaterialPageJsonVo : public JsonVO<InventoryMaterialPageDto::Wrapper> {
	DTO_INIT(InventoryMaterialPageJsonVo, JsonVO<InventoryMaterialPageDto::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INVENTORYMATERIAL_H_
