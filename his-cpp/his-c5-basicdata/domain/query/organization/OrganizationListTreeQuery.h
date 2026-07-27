#ifndef _ORGANIZATIONLISTTREEQUERY_H_
#define _ORGANIZATIONLISTTREEQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationListTreeQuery : public oatpp::DTO
{
    DTO_INIT(OrganizationListTreeQuery, DTO);

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

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        status_enum,
        ZH_WORDS_GETTER("organization.queries.statusEnum")
    );

    API_DTO_FIELD_DEFAULT(
        oatpp::String,
        keyword,
        ZH_WORDS_GETTER("organization.queries.keyword")
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONLISTTREEQUERY_H_
