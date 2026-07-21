#pragma once
#ifndef _FREQUENCYQUERY_H_
#define _FREQUENCYQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class FrequencyQuery : public PageQuery
{
	DTO_INIT(FrequencyQuery, PageQuery);

	// ◊¥Ã¨£®÷ª≤È∆Ù”√£©
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("frequency.field.status"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYQUERY_H_