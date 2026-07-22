#pragma
#ifndef _WARDNAMEVO_H_
#define _WARDNAMEVO_H_
#include"../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class WardNameVO :public oatpp::DTO
{
	DTO_INIT(WardNameVO,DTO);
	DTO_FIELD(String,id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("ward.field.id");
	}
	DTO_FIELD(String,name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("ward.field.name");
	
	}

};
#include OATPP_CODEGEN_END(DTO)
#endif // !1
