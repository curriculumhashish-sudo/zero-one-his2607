#pragma once

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Query parameters used by the sample selection endpoint.
class SampleNameQuery : public oatpp::DTO
{
	DTO_INIT(SampleNameQuery, DTO);

	API_DTO_FIELD_DEFAULT(String, keyword, ZH_WORDS_GETTER("sample.query.keyword"));
};

#include OATPP_CODEGEN_END(DTO)
