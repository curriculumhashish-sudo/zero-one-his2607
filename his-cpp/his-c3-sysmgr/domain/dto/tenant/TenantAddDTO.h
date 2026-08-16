#pragma once
#ifndef _TENANTADDDTO_H_
#define _TENANTADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

//租户新增数据传输对象
class TenantAddDTO :public oatpp::DTO {
	DTO_INIT(TenantAddDTO, DTO);
	//租户编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("tenant.field.code"), true);
	//租户名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("tenant.field.name"), true);
	//租户状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("tenant.field.status"), true);
	//租户描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("tenant.field.description"), false);

	// 关联一个PayloadDTO负载数据对象
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// 数据校验
	std::string validate()
	{
		if (!code || code->empty())
			return "code invalidate.";
		if (!name || name->empty())
			return "name invalidate.";
		if (status!=0&&status!=1)
			return "status invalidate.";
		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_TENANTADDDTO_H_
