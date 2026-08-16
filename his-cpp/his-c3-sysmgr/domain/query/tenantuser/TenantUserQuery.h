#pragma once
#ifndef _TENANTUSERQUERY_H_
#define _TENANTUSERQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TenantUserQuery : public PageQuery
{
	DTO_INIT(TenantUserQuery, PageQuery);
	API_DTO_FIELD_REQUIRE(String, tenantId, ZH_WORDS_GETTER("tenant-user.field.tenantId"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TENANTUSERQUERY_H_
