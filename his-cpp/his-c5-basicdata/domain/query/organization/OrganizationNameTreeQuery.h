#ifndef _ORGANIZATIONNAMETREEQUERY_H_
#define _ORGANIZATIONNAMETREEQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationNameTreeQuery : public oatpp::DTO
{
    DTO_INIT(OrganizationNameTreeQuery, DTO);

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        type_enum,
        ZH_WORDS_GETTER("organization.queries.typeEnum")
    );

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        class_enum,
        ZH_WORDS_GETTER("organization.queries.classEnum")
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONNAMETREEQUERY_H_
