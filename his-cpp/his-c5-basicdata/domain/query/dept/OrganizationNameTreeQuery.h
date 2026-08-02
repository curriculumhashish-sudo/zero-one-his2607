#ifndef _ORGANIZATIONNAMETREEQUERY_H_
#define _ORGANIZATIONNAMETREEQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Optional filters for GET /dept/name-tree.
 *
 * Empty or omitted fields do not participate in filtering. Supplied typeEnum
 * and classEnum values are matched exactly and combined with AND. excludeId
 * is intended for an edit form that must not offer the current organization
 * or any of its descendants as a possible parent. Enum-code filters allow at
 * most 255 characters, and excludeId allows at most 64 characters.
 */
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
