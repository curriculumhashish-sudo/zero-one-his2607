#ifndef _ORGANIZATIONQUERYERRORVO_H_
#define _ORGANIZATIONQUERYERRORVO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationQueryErrorVO : public JsonVO<oatpp::String>
{
    DTO_INIT(OrganizationQueryErrorVO, JsonVO<oatpp::String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONQUERYERRORVO_H_
