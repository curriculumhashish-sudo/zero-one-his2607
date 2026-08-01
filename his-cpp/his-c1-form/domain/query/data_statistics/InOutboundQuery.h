#pragma once
#ifndef _STATICSQUERY_H_
#define _STATICSQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class InOutboundQuery : public PageQuery {
	DTO_INIT(InOutboundQuery, PageQuery);
	

	// 项目名称
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

	// 批号
	DTO_FIELD(String, batch_number);
	DTO_FIELD_INFO(batch_number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number");
	}
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_STATICSQUERY_H_
