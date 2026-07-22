#ifndef _PHARMQUERY_H_
#define _PHARMQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmQuery : public PageQuery
{
	DTO_INIT(PharmQuery, PageQuery);

	// 名称关键词，模糊查询
	API_DTO_FIELD_DEFAULT(
		oatpp::String,
		keyword,
		ZH_WORDS_GETTER("pharm.queries.keyword")
	);

	// 库房/药房类型筛选
	API_DTO_FIELD_DEFAULT(
		oatpp::String,
		type,
		ZH_WORDS_GETTER("pharm.queries.type")
	);

	// 状态筛选
	API_DTO_FIELD_DEFAULT(
		oatpp::Int32,
		status,
		ZH_WORDS_GETTER("pharm.queries.status")
	);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PHARMQUERY_H_