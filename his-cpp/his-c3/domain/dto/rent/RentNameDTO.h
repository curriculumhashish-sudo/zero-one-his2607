#pragma once
#ifndef  _RENTNAME_DTO_
#define  _RENTNAME_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RentNameDTO :public oatpp::DTO {
	DTO_INIT(RentNameDTO, DTO);

	API_DTO_FIELD_DEFAULT(Int32, id, ZH_WORDS_GETTER("rent.field.id"));
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("rent.field.name"));
	API_DTO_FIELD_DEFAULT(String, code, ZH_WORDS_GETTER("rent.field.code"));
};

class RentNamePageDTO : public PageDTO<RentNameDTO::Wrapper>
{
	DTO_INIT(RentNamePageDTO, PageDTO<RentNameDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // ! _RENTNAME_DTO_