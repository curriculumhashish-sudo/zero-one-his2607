#include "stdafx.h"
#include "TenantUserController.h"
#include "service/tenantuser/TenantUserService.h"

TenantUserPageJsonVO::Wrapper TenantUserController::executeQueryTenantUser(const TenantUserQuery::Wrapper& query)
{
	auto jvo = TenantUserPageJsonVO::createShared();
	if (!query || !query->tenantId || query->tenantId->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	jvo->success(TenantUserService().listAll(query));
	return jvo;
}

ListJsonVO<String>::Wrapper TenantUserController::execAddTenantUser(const TenantUserBatchAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = ListJsonVO<String>::createShared();
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	dto->setPayload(&payload);
	auto result = TenantUserService().saveBatch(dto);
	if (result.success)
		jvo->success(result.userIds);
	else
		jvo->fail(result.userIds);
	return jvo;
}

ListJsonVO<String>::Wrapper TenantUserController::execRemoveTenantUser(const TenantUserBatchRemoveDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = ListJsonVO<String>::createShared();
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	dto->setPayload(&payload);
	auto result = TenantUserService().removeBatch(dto);
	if (result.success)
		jvo->success(result.userIds);
	else
		jvo->fail(result.userIds);
	return jvo;
}
