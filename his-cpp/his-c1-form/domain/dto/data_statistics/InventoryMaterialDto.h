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
#ifndef _INVENTORYMATERIALDTO_H_
#define _INVENTORYMATERIALDTO_H_

#include "../../GlobalInclude.h"
#include "ApiHelper.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/PageDTO.h"
#include "domain/query/PageQuery.h"
#include "domain/query/data_statistics/InventoryMaterialQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class InventoryMaterialAddDto : public oatpp::DTO {
	DTO_INIT(InventoryMaterialAddDto, DTO);

	// Expiration date
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

public:
	// Field validation
	std::string isValid() {
		if (!time || time->empty()) {
			return "time is invalid";
		}
		if (!numbers || numbers < 0) {
			return "numbers is invalid";
		}
		if (!batch_number || batch_number->empty()) {
			return "batch_number is invalid";
		}
		return "";
	}
};

// Detail DTO (extends add DTO with ID)
class InventoryMaterialDto : public InventoryMaterialAddDto {
	DTO_INIT(InventoryMaterialDto, InventoryMaterialAddDto);

	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.id");
	}
};

// Paged result DTO
class InventoryMaterialPageDto : public PageDTO<InventoryMaterialDto::Wrapper> {
	DTO_INIT(InventoryMaterialPageDto, PageDTO<InventoryMaterialDto::Wrapper>);
};


class InventoryMaterialimportDTO : public oatpp::DTO {
	DTO_INIT(InventoryMaterialimportDTO, DTO);
	// Import report file
	API_DTO_FIELD_REQUIRE(oatpp::swagger::Binary, excel, ZH_WORDS_GETTER("Litguidyo.form.inoutbound.excel"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INVENTORYMATERIALDTO_H_
