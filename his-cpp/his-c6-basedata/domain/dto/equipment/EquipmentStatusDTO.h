#pragma once

#ifndef _EQUIPMENTSTATUSDTO_H_
#define _EQUIPMENTSTATUSDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentStatusDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentStatusDTO, DTO);

	// 要修改状态的器材ID集合
	API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("equipment.field.ids"), true);
	// 目标状态 (1启用 0停用)
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("equipment.field.status"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif 