#pragma once


#ifndef _GET_SEE_QUERY_
#define _GET_SEE_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSeeQuery:public PageQuery
{
	DTO_INIT(GetSeeQuery, PageQuery);

	//¾ÍÕïºÅ
	DTO_FIELD(String, number);
	DTO_FIELD_INFO(number){
		info->description= ZH_WORDS_GETTER("yxy.charging.reister.number");
	}

	//»¼ÕßÐÕÃû
	DTO_FIELD(String, register_type);
	DTO_FIELD_INFO(register_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging.reister.register_type");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif 