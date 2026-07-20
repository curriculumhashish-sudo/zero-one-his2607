#ifndef _ORGANIZATIONLISTTREEQUERY_H_
#define _ORGANIZATIONLISTTREEQUERY_H_

#include "../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationListTreeQuery : public oatpp::DTO
{
    DTO_INIT(OrganizationListTreeQuery, DTO);

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

    API_DTO_FIELD_DEFAULT(
        String,
        status_enum,
        "Organization status filter"
    );

    API_DTO_FIELD_DEFAULT(
        String,
        keyword,
        "Keyword for organization and insurance fields"
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONLISTTREEQUERY_H_