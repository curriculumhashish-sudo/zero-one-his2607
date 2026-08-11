#pragma once
#ifndef  _RENTDETAILDTO_H_
#define  _RENTDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RentDetailDTO :public oatpp::DTO {
	DTO_INIT(RentDetailDTO, DTO);

	API_DTO_FIELD_DEFAULT(Int32, id, ZH_WORDS_GETTER("rent.field.id"));
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("rent.field.name"));
	API_DTO_FIELD_DEFAULT(String, code, ZH_WORDS_GETTER("rent.field.code"));
	API_DTO_FIELD_DEFAULT(String, description, ZH_WORDS_GETTER("rent.field.description"));
	API_DTO_FIELD_DEFAULT(Int32, is_active, ZH_WORDS_GETTER("rent.field.is_active"));
	API_DTO_FIELD_DEFAULT(String, extraProperties, ZH_WORDS_GETTER("rent.field.extraProperties"));
	API_DTO_FIELD_DEFAULT(String, created_at, ZH_WORDS_GETTER("rent.field.created_at"));
	API_DTO_FIELD_DEFAULT(String, updated_at, ZH_WORDS_GETTER("rent.field.updated_at"));
	API_DTO_FIELD_DEFAULT(String, created_by, ZH_WORDS_GETTER("rent.field.created_by"));
	API_DTO_FIELD_DEFAULT(String, updated_by, ZH_WORDS_GETTER("rent.field.updated_by"));
	API_DTO_FIELD_DEFAULT(Int32, is_deleted, ZH_WORDS_GETTER("rent.field.is_deleted"));
	API_DTO_FIELD_DEFAULT(String, deleted_at, ZH_WORDS_GETTER("rent.field.deleted_at"));
	API_DTO_FIELD_DEFAULT(String, deleted_by, ZH_WORDS_GETTER("rent.field.deleted_by"));
};

class RentDetailPageDTO : public PageDTO<RentDetailDTO::Wrapper>
{
	DTO_INIT(RentDetailPageDTO, PageDTO<RentDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // ! _RENTDETAILDTO_H_

