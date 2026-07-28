#ifndef _ORGANIZATIONDETAILVO_H_
#define _ORGANIZATIONDETAILVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/dept/OrganizationDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Success envelope for one organization detail object.
 * A missing organization is returned by the controller as HTTP 404 with the
 * dedicated error envelope instead of a successful response with null data.
 */
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
