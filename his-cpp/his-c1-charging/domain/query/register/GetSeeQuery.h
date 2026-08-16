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
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("yxy.charging.reister.name");
	}

	DTO_FIELD(UInt64, start_time);
	DTO_FIELD_INFO(start_time) {
		info->description = ZH_WORDS_GETTER("yxy.charging.reister.start_time");
	}


	DTO_FIELD(UInt64, end_time);
	DTO_FIELD_INFO(end_time) {
		info->description = ZH_WORDS_GETTER("yxy.charging.reister.end_time");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif 