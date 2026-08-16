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
#ifndef _INOUTBOUNDDTO_H_
#define _INOUTBOUNDDTO_H_

#include "../../GlobalInclude.h"
#include "ApiHelper.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/PageDTO.h"
#include "domain/query/PageQuery.h"
#include "domain/query/data_statistics/InOutboundQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class InOutboundAddDto : public oatpp::DTO {
	DTO_INIT(InOutboundAddDto, DTO);

	// Item name
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.name");
	}

	// 入库单价
	DTO_FIELD(Float64, onemoney);
	DTO_FIELD_INFO(onemoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.onemoney");
	}

	// 入库总金额
	DTO_FIELD(Float64, allmoney);
	DTO_FIELD_INFO(allmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.allmoney");
	}

	// 出库单价
	DTO_FIELD(Float64, outmoney);
	DTO_FIELD_INFO(outmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.outmoney");
	}

	// 出库总金额
	DTO_FIELD(Float64, alloutmoney);
	DTO_FIELD_INFO(alloutmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.alloutmoney");
	}

	// Batch number
	DTO_FIELD(String, batch_number);
	DTO_FIELD_INFO(batch_number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number");
	}

public:
	// Field validation
	std::string isValid() {
		if (!name || name->empty()) {
			return "name is invalid";
		}
		if (!onemoney || onemoney < 0) {
			return "onemoney is invalid";
		}
		if (!allmoney || allmoney < 0) {
			return "allmoney is invalid";
		}
		if (!batch_number || batch_number->empty()) {
			return "batch_number is invalid";
		}
		if (!outmoney || outmoney < 0) {
			return "outmoney is invalid";
		}
		if (!alloutmoney || alloutmoney < 0) {
			return "alloutmoney is invalid";
		}
		return "";
	}
};

// Detail DTO (extends add DTO with ID)
class InOutboundDto : public InOutboundAddDto {
	DTO_INIT(InOutboundDto, InOutboundAddDto);

	DTO_FIELD(String, id); // Record ID
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.id");
	}
};

// Paged result DTO
class InOutboundPageDto : public PageDTO<InOutboundDto::Wrapper> {
	DTO_INIT(InOutboundPageDto, PageDTO<InOutboundDto::Wrapper>);
};


class InOutboundimportDTO : public oatpp::DTO {
	DTO_INIT(InOutboundimportDTO, DTO);
	// Import report file
	API_DTO_FIELD_REQUIRE(oatpp::swagger::Binary, excel, ZH_WORDS_GETTER("Litguidyo.form.inoutbound.excel"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INOUTBOUNDDTO_H_