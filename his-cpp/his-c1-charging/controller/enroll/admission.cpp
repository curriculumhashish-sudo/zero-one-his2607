#include "stdafx.h"
#include "admission.h"
#include "../../service/enroll/AdmissionService.h"

StringJsonVO::Wrapper AdmissionController::admission_add(const AdmissionAddDTO::Wrapper& dto)
{
	// 构建统一的JSON响应
	auto result = StringJsonVO::createShared();

	// 校验必填字段
	std::string errMsg = dto->validate();
	if (!errMsg.empty())
	{
		result->init(oatpp::String(errMsg), RS_PARAMS_INVALID);
		return result;
	}

	// 调用服务执行入院登记
	AdmissionService service;
	bool success = service.insertAdmission(dto, *dto->getPayload());

	if (success)
	{
		result->success(oatpp::String("Admission registration successful"));
	}
	else
	{
		result->fail(oatpp::String("Admission registration failed"));
	}
	return result;
}

StringJsonVO::Wrapper AdmissionController::admission_delete(
	const AdmissionDeleteDTO::Wrapper& dto
)
{
	// 构建统一的JSON响应
	auto result = StringJsonVO::createShared();

	// 校验必填字段
	std::string errMsg = dto->validate();
	if (!errMsg.empty())
	{
		result->init(oatpp::String(errMsg), RS_PARAMS_INVALID);
		return result;
	}

	// 调用服务执行入院取消
	AdmissionService service;
	bool success = service.deleteAdmission(dto, *dto->getPayload());

	if (success)
	{
		result->success(oatpp::String("Admission cancellation successful"));
	}
	else
	{
		result->fail(oatpp::String("Admission cancellation failed"));
	}
	return result;
}
