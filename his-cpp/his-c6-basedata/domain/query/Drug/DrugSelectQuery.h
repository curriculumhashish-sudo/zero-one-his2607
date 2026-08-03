#pragma once
#ifndef _DRUGSELECTQUERY_H_
#define  _DRUGSELECTQUERY_H_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class DrugSelectQuery : public PageQuery
{
	DTO_INIT(DrugSelectQuery, PageQuery);
	API_DTO_FIELD_DEFAULT(String, proname, ZH_WORDS_GETTER("Drug.field.name"), false);
	//名称
	API_DTO_FIELD_DEFAULT(String, itemMinQuantity ,ZH_WORDS_GETTER("Drug.field.itemMinQuantity"), false);
	API_DTO_FIELD_DEFAULT(String, itemMaxQuantity, ZH_WORDS_GETTER("Drug.field.itemMaxQuantity"), false);
	API_DTO_FIELD_DEFAULT(String, yongfa, ZH_WORDS_GETTER("Drug.field.yongfa"), false);
	
	//分类
};
#include OATPP_CODEGEN_END(DTO);
#endif // !