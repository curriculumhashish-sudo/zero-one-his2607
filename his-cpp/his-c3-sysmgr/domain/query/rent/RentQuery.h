#pragma once
#ifndef  _RENTQUERY_H_
#define _RENTQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RentQuery : public PageQuery {
	DTO_INIT(RentQuery,PageQuery);
	// ×â»§±àÂë
	API_DTO_FIELD_DEFAULT(String,code,ZH_WORDS_GETTER("rent.field.code"),false);
	// ¹Ø¼ü´Ê
	API_DTO_FIELD_DEFAULT(String, keywords, ZH_WORDS_GETTER("rent.field.keywords"), false);
	// ×â»§×´Ì¬
	API_DTO_FIELD_DEFAULT(Int32, status, ZH_WORDS_GETTER("rent.field.status"), false);
};


#include OATPP_CODEGEN_END(DTO)
#endif // ! _RENTQUERY_H_
