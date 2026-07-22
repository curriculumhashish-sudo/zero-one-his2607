#pragma once
#ifndef _INVENTORYMATERIAL_H_
#define _INVENTORYMATERIAL_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class InventoryMaterialQuery : public PageQuery {
	DTO_INIT(InventoryMaterialQuery, PageQuery);

	// 有效期
	DTO_FIELD(String, time);
	DTO_FIELD_INFO(time) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.time");
	}

	// 该有效期对应的数量
	DTO_FIELD(Int32, numbers);
	DTO_FIELD_INFO(numbers) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.numbers");
	}

	// 批号
	DTO_FIELD(String, batch_number);
	DTO_FIELD_INFO(batch_number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INVENTORYMATERIAL_H_
