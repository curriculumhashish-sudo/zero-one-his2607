#pragma once
#ifndef _TENANTUSERCONTROLLER_H_
#define _TENANTUSERCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/tenantuser/TenantUserQuery.h"
#include "domain/vo/tenantuser/TenantUserVO.h"
#include "domain/dto/tenantuser/TenantUserBatchAddDTO.h"
#include "domain/dto/tenantuser/TenantUserBatchRemoveDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("tenant-user.tags.tag")

class TenantUserController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(TenantUserController);
public:
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("tenant-user.query.summary"), queryTenantUser, TenantUserQuery, TenantUserPageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/tenant-user/query", queryTenantUser, TenantUserQuery, executeQueryTenantUser(query));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tenant-user.add.summary"), addTenantUser, ListJsonVO<String>::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tenant-user/add", addTenantUser, BODY_DTO(TenantUserBatchAddDTO::Wrapper, dto), execAddTenantUser(dto, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tenant-user.remove.summary"), removeTenantUser, ListJsonVO<String>::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/tenant-user/remove", removeTenantUser, BODY_DTO(TenantUserBatchRemoveDTO::Wrapper, dto), execRemoveTenantUser(dto, authObject->getPayload()));
private:
	TenantUserPageJsonVO::Wrapper executeQueryTenantUser(const TenantUserQuery::Wrapper& query);
	ListJsonVO<String>::Wrapper execAddTenantUser(const TenantUserBatchAddDTO::Wrapper& dto, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemoveTenantUser(const TenantUserBatchRemoveDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_TENANTUSERCONTROLLER_H_
