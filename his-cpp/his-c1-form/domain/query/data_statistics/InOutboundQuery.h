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

	// 单价
	DTO_FIELD(Int32, onemoney);
	DTO_FIELD_INFO(onemoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.onemoney");
	}

	// 总金额
	DTO_FIELD(Int32, allmoney);
	DTO_FIELD_INFO(allmoney) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.allmoney");
	}

	// 数量
	DTO_FIELD(Int32, number);
	DTO_FIELD_INFO(number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.number");
	}

	// 厂家
	DTO_FIELD(String, origin);
	DTO_FIELD_INFO(origin) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.origin");
	}

	// 批号
	DTO_FIELD(String, batch_number);
	DTO_FIELD_INFO(batch_number) {
		info->description = ZH_WORDS_GETTER("Litguidyo.form.inoutbound.batch_number");
	}
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_STATICSQUERY_H_
