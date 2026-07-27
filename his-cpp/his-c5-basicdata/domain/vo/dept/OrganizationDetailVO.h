#ifndef _ORGANIZATIONDETAILVO_H_
#define _ORGANIZATIONDETAILVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/dept/OrganizationDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationDetailJsonVO
    : public JsonVO<OrganizationDetailDTO::Wrapper>
{
    DTO_INIT(
        OrganizationDetailJsonVO,
        JsonVO<OrganizationDetailDTO::Wrapper>
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONDETAILVO_H_
