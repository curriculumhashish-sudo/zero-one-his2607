#pragma once
#ifndef  _DRUGSelectDTO_H_
#define  _DRUGSelectDTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class DrugSelectDTO:public oatpp::DTO
{
	DTO_INIT(DrugSelectDTO, DTO);
	API_DTO_FIELD_DEFAULT(String, code, ZH_WORDS_GETTER("Drug.field.code"), true);
	//关键词
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("Drug.field.name"), true);
	//名称
	API_DTO_FIELD_DEFAULT(String, danwei, ZH_WORDS_GETTER("Drug.field.danwei"), true);

};
class DrugSelectPageDTO :public PageDTO< DrugSelectDTO::Wrapper>
{
	DTO_INIT(DrugSelectPageDTO, PageDTO<DrugSelectDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO);
#endif // !