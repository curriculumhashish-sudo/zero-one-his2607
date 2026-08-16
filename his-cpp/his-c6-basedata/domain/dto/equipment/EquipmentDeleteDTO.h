#pragma once

#ifndef _EQUIPMENTDELETEDTO_H_
#define _EQUIPMENTDELETEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentDeleteDTO : public oatpp::DTO
{
	DTO_INIT(EquipmentDeleteDTO, DTO);

	// 要删除的器材ID集合
	API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("equipment.field.ids"), true);

public:
	// 校验批量删除参数
	std::string validate() const
	{
		// 校验List是否为null或者数量为0
		if (!ids || ids->empty()) {
			return "要删除的器材ID集合不能为空";
		}
		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif 