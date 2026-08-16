#pragma once
#ifndef PAGEDIAGNOSISDICTIONARYDTO_H
#define PAGEDIAGNOSISDICTIONARYDTO_H

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PageDiagnosisDictionaryDTO : public oatpp::DTO
{
public:
    DTO_INIT(PageDiagnosisDictionaryDTO, DTO);

    //诊断id
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("dict.dto.id"), false);

    //诊断编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("dict.dto.code"), false);

	//诊断名称
    API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("dict.dto.name"), false);

    //诊断分类
    API_DTO_FIELD_REQUIRE(String, typeName, ZH_WORDS_GETTER("dict.dto.type"), false);

};

//定义一个分页的DTO
class PageDiagnosisDictionaryPageDTO : public PageDTO<PageDiagnosisDictionaryDTO::Wrapper>
{
    DTO_INIT(PageDiagnosisDictionaryPageDTO, PageDTO<PageDiagnosisDictionaryDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)


#endif // !
