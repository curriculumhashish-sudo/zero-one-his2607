#pragma once
#ifndef _TENANTCONTROLLER_H_
#define _TENANTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/tenant/TenantAddDTO.h"
#include "domain/dto/tenant/TenantModifyDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("tenant.tags.tag")

class TenantController : public oatpp::web::server::api::ApiController {
	//定义控制器访问入口
	API_ACCESS_DECLARE(TenantController);
public:
	//定义新增租户接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tenant.add.summary"), addTenant, StringJsonVO::Wrapper, API_TAG);
	//定义新增租户接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tenant/add-tenant", addTenant, BODY_DTO(TenantAddDTO::Wrapper, dto), execAddTenant(dto, authObject->getPayload()));

	//定义修改租户接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tenant.modify.summary"), modifyTenant, StringJsonVO::Wrapper, API_TAG);
	//定义修改租户接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/tenant/modify-tenant", modifyTenant, BODY_DTO(TenantModifyDTO::Wrapper, dto), execModifyTenant(dto, authObject->getPayload()));

	//定义删除租户接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tenant.remove.summary"), removeTenant, BooleanJsonVO::Wrapper, API_TAG);
	//定义删除租户接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/tenant/remove-tenant/{id}", removeTenant, PATH(String, id), execRemoveTenant(id, authObject->getPayload()));
	private:
	//定义新增租户执行函数
	StringJsonVO::Wrapper execAddTenant(const TenantAddDTO::Wrapper& dto, const PayloadDTO& payload);
	//定义修改租户执行函数
	StringJsonVO::Wrapper execModifyTenant(const TenantModifyDTO::Wrapper& dto, const PayloadDTO& payload);
	//定义删除租户执行函数
	BooleanJsonVO::Wrapper execRemoveTenant(const String& id, const PayloadDTO& payload);

};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_TENANTCONTROLLER_H_
