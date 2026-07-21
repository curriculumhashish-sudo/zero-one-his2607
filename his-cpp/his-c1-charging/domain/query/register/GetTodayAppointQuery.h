#pragma once


#ifndef _GET_TODAY_APPOINT_QUERY_
#define _GET_TODAY_APPOINT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetTodayAppointQuery :public PageQuery
{
	DTO_INIT(GetTodayAppointQuery, PageQuery);


	DTO_FIELD(String, docker);
	DTO_FIELD_INFO(docker) {
		info->description = ZH_WORDS_GETTER("yxy.charging.reister.docker");
	}

	DTO_FIELD(String, register_type);
	DTO_FIELD_INFO(register_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging.reister.register_type");
	}

};

#include OATPP_CODEGEN_END(DTO)
#endif 