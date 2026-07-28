#pragma once
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

	// 项目名称
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.name");
	}

	// 入库单价
	DTO_FIELD(Int32, onemoney);
	DTO_FIELD_INFO(onemoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.onemoney");
	}

	// 入库总金额
	DTO_FIELD(Int32, allmoney);
	DTO_FIELD_INFO(allmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.allmoney");
	}

	// 出库单价
	DTO_FIELD(Int32, outmoney);
	DTO_FIELD_INFO(outmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.outmoney");
	}

	// 出库总金额
	DTO_FIELD(Int32, alloutmoney);
	DTO_FIELD_INFO(alloutmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.alloutmoney");
	}

	// 批号
	DTO_FIELD(String, batch_number);
	DTO_FIELD_INFO(batch_number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number");
	}

public:
	// 数据校验
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
		if (!alloutmoney || outmoney < 0) {
			return "alloutmoney is invalid";
		}
		return "";
	}
};

// 传输对象
class InOutboundDto : public InOutboundAddDto {
	DTO_INIT(InOutboundDto, InOutboundAddDto);

	DTO_FIELD(String, id); // 入库药品的编号
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.id");
	}
};

// 分页传输对象
class InOutboundPageDto : public PageDTO<InOutboundDto::Wrapper> {
	DTO_INIT(InOutboundPageDto, PageDTO<InOutboundDto::Wrapper>);
};


class InOutboundimportDTO : public oatpp::DTO {
	DTO_INIT(InOutboundimportDTO, DTO);
	// 导入报表文件
	API_DTO_FIELD_REQUIRE(oatpp::swagger::Binary, excel, ZH_WORDS_GETTER("Litguidyo.form.inoutbound.excel"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INOUTBOUNDDTO_H_
