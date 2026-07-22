#pragma once
#ifndef _TENANTUSERBATCHREMOVEDTO_H_
#define _TENANTUSERBATCHREMOVEDTO_H_

#include "TenantUserBatchAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TenantUserBatchRemoveDTO : public TenantUserBatchAddDTO
{
	DTO_INIT(TenantUserBatchRemoveDTO, TenantUserBatchAddDTO);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TENANTUSERBATCHREMOVEDTO_H_
