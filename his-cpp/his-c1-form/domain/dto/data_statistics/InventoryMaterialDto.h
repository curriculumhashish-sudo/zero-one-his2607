#pragma once
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

public:
	// 数据校验
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

// 传输对象
class InventoryMaterialDto : public InventoryMaterialAddDto {
	DTO_INIT(InventoryMaterialDto, InventoryMaterialAddDto);

	DTO_FIELD(String, id); 
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.id");
	}
};

// 分页传输对象
class InventoryMaterialPageDto : public PageDTO<InventoryMaterialDto::Wrapper> {
	DTO_INIT(InventoryMaterialPageDto, PageDTO<InventoryMaterialDto::Wrapper>);
};


class InventoryMaterialimportDTO : public oatpp::DTO {
	DTO_INIT(InventoryMaterialimportDTO, DTO);
	// 导入报表文件
	API_DTO_FIELD_REQUIRE(oatpp::swagger::Binary, excel, ZH_WORDS_GETTER("Litguidyo.form.inoutbound.excel"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _INVENTORYMATERIALDTO_H_
