#pragma once
#ifndef _DRUGQUERY_H_
#define  _DRUGQUERY_H_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class DrugQuery : public PageQuery
{
	DTO_INIT(DrugQuery, PageQuery);
	API_DTO_FIELD_DEFAULT(String, keywords, ZH_WORDS_GETTER("Drug.field.keywords"), false);
	//关键词
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("Drug.field.name"), false);
	//名称
	API_DTO_FIELD_DEFAULT(String, kind, ZH_WORDS_GETTER("Drug.field.kind"), false);
	//分类
};
#include OATPP_CODEGEN_END(DTO);
#endif // !
