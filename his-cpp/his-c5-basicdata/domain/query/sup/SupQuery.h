#pragma once

#ifndef __SUP_QUERY_
#define __SUP_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupQuery : public oatpp::DTO
{
	DTO_INIT(SupQuery, DTO)

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("sup.field.name");
	}
	
	DTO_FIELD(UInt64, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("sup.field.id");
	}
	

};


#include OATPP_CODEGEN_END(DTO)

#endif 