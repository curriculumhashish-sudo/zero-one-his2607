#pragma once
#ifndef  _RENTQUERY_H_
#define _RENTQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RentQuery : public PageQuery {
	DTO_INIT(RentQuery,PageQuery);
	// �⻧����
	API_DTO_FIELD_DEFAULT(String,code,ZH_WORDS_GETTER("rent.field.code"));
	// �ؼ���
	API_DTO_FIELD_DEFAULT(String, keywords, ZH_WORDS_GETTER("rent.field.keywords"));
	// �⻧״̬
	API_DTO_FIELD_DEFAULT(Int32, status, ZH_WORDS_GETTER("rent.field.status"));
};


#include OATPP_CODEGEN_END(DTO)
#endif // ! _RENTQUERY_H_
