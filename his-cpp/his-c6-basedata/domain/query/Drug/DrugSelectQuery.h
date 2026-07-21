#pragma once
#ifndef _DRUGSELECTQUERY_H_
#define  _DRUGSELECTQUERY_H_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class DrugSelectQuery : public PageQuery
{
	DTO_INIT(DrugSelectQuery, DTO);
	API_DTO_FIELD_DEFAULT(String, keywords, ZH_WORDS_GETTER("Drug.field.keywords"), false);
	//关键词
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("Drug.field.name"), false);
	//名称
	API_DTO_FIELD_DEFAULT(Int32, kind, ZH_WORDS_GETTER("Drug.field.kind"), false);
	//分类
};
#include OATPP_CODEGEN_END(DTO);
#endif // !