#include "stdafx.h"
#include "TenantController.h"
#include "../../service/tenant/TenantService.h"

// 定义新增租户执行函数
StringJsonVO::Wrapper TenantController::execAddTenant(const TenantAddDTO::Wrapper& dto, const PayloadDTO& payload) {
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 参数校验
	std::string errmsg = dto->validate();

	if (errmsg != "") {
		jvo->init(errmsg, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据新增
	dto->setPayload(&payload);
	std::string id = TenantService().saveData(dto);
	if (id != "") {
		jvo->success(id);
	}
	else
	{
		jvo->fail("");
	}
	//响应结果
	return jvo;
}

// 定义修改租户执行函数
StringJsonVO::Wrapper TenantController::execModifyTenant(const TenantModifyDTO::Wrapper& dto, const PayloadDTO& payload) {
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 参数校验
	std::string errmsg = dto->validate();
	if (errmsg != "") {
		jvo->init(errmsg, RS_PARAMS_INVALID);
		return jvo;
	}

	// 设置负载数据
	dto->setPayload(&payload);
	// 执行数据修改
	if (TenantService().updateData(dto)) {
		jvo->success(dto->id);
	}
	else {
		jvo->fail("");
	}
	// 响应结果
	return jvo;
}

// 定义删除租户执行函数
BooleanJsonVO::Wrapper TenantController::execRemoveTenant(const String& id, const PayloadDTO& payload) {
	// 定义返回数据对象
	auto bvo = BooleanJsonVO::createShared();
	// 执行软删除,传入操作人用户名
	if (TenantService().removeData(id.getValue(""), payload.getUsername())) {
		bvo->success(true);
	}
	else {
		bvo->fail("");
	}
	// 响应结果
	return bvo;
}