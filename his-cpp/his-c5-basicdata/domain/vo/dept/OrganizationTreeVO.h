#ifndef _ORGANIZATIONTREEVO_H_
#define _ORGANIZATIONTREEVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/dept/OrganizationNameTreeDTO.h"
#include "../../dto/dept/OrganizationListTreeDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Success envelope for the selectable name tree.
 * data is always initialized as an array, so a successful empty query is [].
 */
class OrganizationNameTreeJsonVO
    : public JsonVO<oatpp::List<OrganizationNameTreeDTO::Wrapper>>
{
    DTO_INIT(
        OrganizationNameTreeJsonVO,
        JsonVO<oatpp::List<OrganizationNameTreeDTO::Wrapper>>
    );

public:
    OrganizationNameTreeJsonVO()
    {
        data = {};
    }
};

/**
 * Success envelope for the organization management tree.
 * data is always initialized as an array, so a successful empty query is [].
 */
class OrganizationListTreeJsonVO
    : public JsonVO<oatpp::List<OrganizationListTreeDTO::Wrapper>>
{
    DTO_INIT(
        OrganizationListTreeJsonVO,
        JsonVO<oatpp::List<OrganizationListTreeDTO::Wrapper>>
    );

    DTO_FIELD(oatpp::Boolean, enumMappingSuccess) = true;
    DTO_FIELD_INFO(enumMappingSuccess)
    {
        info->description = ZH_WORDS_GETTER(
            "dept.organizationQuery.fields.enumMappingSuccess"
        );
        info->required = true;
    }

public:
    OrganizationListTreeJsonVO()
    {
        data = {};
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONTREEVO_H_
