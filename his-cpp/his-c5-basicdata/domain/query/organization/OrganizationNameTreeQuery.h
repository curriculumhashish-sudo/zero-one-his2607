#ifndef _ORGANIZATIONNAMETREEQUERY_H_
#define _ORGANIZATIONNAMETREEQUERY_H_

#include "../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationNameTreeQuery : public oatpp::DTO
{
    DTO_INIT(OrganizationNameTreeQuery, DTO);

    API_DTO_FIELD_DEFAULT(
        String,
        type_enum,
        "Organization type filter"
    );

    API_DTO_FIELD_DEFAULT(
        String,
        class_enum,
        "Organization category filter"
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONNAMETREEQUERY_H_