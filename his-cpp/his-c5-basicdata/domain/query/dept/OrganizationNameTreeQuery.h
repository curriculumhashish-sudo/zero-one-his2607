#ifndef _ORGANIZATIONNAMETREEQUERY_H_
#define _ORGANIZATIONNAMETREEQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationNameTreeQuery : public oatpp::DTO
{
    DTO_INIT(OrganizationNameTreeQuery, DTO);

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        typeEnum,
        ZH_WORDS_GETTER("dept.organizationQuery.queries.typeEnum")
    );

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        classEnum,
        ZH_WORDS_GETTER("dept.organizationQuery.queries.classEnum")
    );

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        excludeId,
        ZH_WORDS_GETTER("dept.organizationQuery.queries.excludeId")
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONNAMETREEQUERY_H_
