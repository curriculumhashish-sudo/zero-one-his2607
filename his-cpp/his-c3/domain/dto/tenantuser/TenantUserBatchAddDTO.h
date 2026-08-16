#pragma once
#ifndef _TENANTUSERBATCHADDDTO_H_
#define _TENANTUSERBATCHADDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TenantUserBatchAddDTO : public oatpp::DTO
{
	DTO_INIT(TenantUserBatchAddDTO, DTO);
	API_DTO_FIELD_REQUIRE(String, tenantId, ZH_WORDS_GETTER("tenant-user.field.tenantId"), true);
	API_DTO_FIELD_REQUIRE(List<String>, userIds, ZH_WORDS_GETTER("tenant-user.field.userIds"), true);

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	std::string validate()
	{
		if (!tenantId || tenantId->empty())
			return "tenantId invalidate.";
		if (!userIds || userIds->empty())
			return "userIds invalidate.";
		for (auto userId : *userIds.get())
		{
			if (!userId || userId->empty())
				return "userIds invalidate.";
		}
		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TENANTUSERBATCHADDDTO_H_
