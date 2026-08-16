#pragma once
#ifndef _TENANTUSERDTO_H_
#define _TENANTUSERDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TenantUserDTO : public oatpp::DTO
{
	DTO_INIT(TenantUserDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("tenant-user.field.id"), true);
	API_DTO_FIELD_REQUIRE(String, tenantId, ZH_WORDS_GETTER("tenant-user.field.tenantId"), true);
	API_DTO_FIELD_REQUIRE(String, userId, ZH_WORDS_GETTER("tenant-user.field.userId"), true);
	API_DTO_FIELD_REQUIRE(String, userName, ZH_WORDS_GETTER("tenant-user.field.userName"), true);
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("tenant-user.field.name"), false);
	API_DTO_FIELD_REQUIRE(String, phoneNumber, ZH_WORDS_GETTER("tenant-user.field.phoneNumber"), false);
	API_DTO_FIELD_REQUIRE(String, email, ZH_WORDS_GETTER("tenant-user.field.email"), false);
	API_DTO_FIELD_REQUIRE(Int32, isActive, ZH_WORDS_GETTER("tenant-user.field.isActive"), true);
	API_DTO_FIELD_REQUIRE(String, createdAt, ZH_WORDS_GETTER("tenant-user.field.createdAt"), false);
	API_DTO_FIELD_REQUIRE(String, createdBy, ZH_WORDS_GETTER("tenant-user.field.createdBy"), false);
};

class TenantUserPageDTO : public PageDTO<TenantUserDTO::Wrapper>
{
	DTO_INIT(TenantUserPageDTO, PageDTO<TenantUserDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TENANTUSERDTO_H_
