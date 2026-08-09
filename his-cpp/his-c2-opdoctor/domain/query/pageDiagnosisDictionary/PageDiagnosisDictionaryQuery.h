#pragma once
#ifndef PAGE_DIAGNOSISDICTIONARY_H
#define PAGE_DIAGNOSISDICTIONARY_H

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"


#include OATPP_CODEGEN_BEGIN(DTO)

class PageDiagnosisDictionaryQuery : public PageQuery
{
	DTO_INIT(PageDiagnosisDictionaryQuery, PageQuery);

	API_DTO_FIELD_REQUIRE(String,keyword,ZH_WORDS_GETTER("dict.query.keyword"),false);

};



#include OATPP_CODEGEN_END(DTO)


#endif