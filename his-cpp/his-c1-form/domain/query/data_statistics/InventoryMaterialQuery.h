#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _INVENTORYMATERIAL_H_
#define _INVENTORYMATERIAL_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class InventoryMaterialQuery : public PageQuery {
	DTO_INIT(InventoryMaterialQuery, PageQuery);

	// Expiration date (date string)
	DTO_FIELD(String, time);
	DTO_FIELD_INFO(time) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.time");
	}

	// Valid material count
	DTO_FIELD(Int32, numbers);
	DTO_FIELD_INFO(numbers) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.numbers");
	}

	// Batch number
	DTO_FIELD(String, batch_number);
	DTO_FIELD_INFO(batch_number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INVENTORYMATERIAL_H_
