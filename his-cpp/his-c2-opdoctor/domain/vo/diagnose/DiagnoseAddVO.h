#pragma once

#include "domain/GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DiagnoseAddVO : oatpp::DTO
{
    DTO_INIT(DiagnoseAddVO, DTO)

    API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("diagnose.add.id"));
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("diagnose.add.encounterId"));
    API_DTO_FIELD_DEFAULT(String, description, ZH_WORDS_GETTER("diagnose.add.description"));
    API_DTO_FIELD_DEFAULT(Int8, primaryFlag, ZH_WORDS_GETTER("diagnose.add.primaryFlag"));
    API_DTO_FIELD_DEFAULT(String, createdAt, ZH_WORDS_GETTER("diagnose.add.createdAt"));
};

class DiagnoseAddJsonVO : public JsonVO<DiagnoseAddVO::Wrapper>
{
	DTO_INIT(DiagnoseAddJsonVO, JsonVO<DiagnoseAddVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)