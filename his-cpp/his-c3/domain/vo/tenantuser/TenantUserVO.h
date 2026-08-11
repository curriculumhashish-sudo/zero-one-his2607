#pragma once
#ifndef _TENANTUSERVO_H_
#define _TENANTUSERVO_H_

#include "../../dto/tenantuser/TenantUserDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TenantUserPageJsonVO : public JsonVO<TenantUserPageDTO::Wrapper>
{
	DTO_INIT(TenantUserPageJsonVO, JsonVO<TenantUserPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TENANTUSERVO_H_
