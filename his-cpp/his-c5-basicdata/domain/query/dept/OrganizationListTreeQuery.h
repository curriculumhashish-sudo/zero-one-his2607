#ifndef _ORGANIZATIONLISTTREEQUERY_H_
#define _ORGANIZATIONLISTTREEQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationListTreeQuery : public oatpp::DTO
{
    DTO_INIT(OrganizationListTreeQuery, DTO);

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
        statusEnum,
        ZH_WORDS_GETTER("dept.organizationQuery.queries.statusEnum")
    );

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        keyword,
        ZH_WORDS_GETTER("dept.organizationQuery.queries.keyword")
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONLISTTREEQUERY_H_
