#include "stdafx.h"
#include "admission.h"
#include "../../service/enroll/AdmissionService.h"

StringJsonVO::Wrapper AdmissionController::execAdmissionAdd(const AdmissionAddDTO::Wrapper& dto, const std::shared_ptr<CustomerAuthorizeObject>& authObject)
{
	// Build unified JSON response
	auto result = StringJsonVO::createShared();

	// Validate required fields
	std::string errMsg = dto->validate();
	if (!errMsg.empty())
	{
		result->init(oatpp::String(errMsg), RS_PARAMS_INVALID);
		return result;
	}

	// Call service to execute admission registration
	AdmissionService service;
	bool success = service.insertAdmission(dto, authObject->getPayload());

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

StringJsonVO::Wrapper AdmissionController::execAdmissionDelete(
		const AdmissionDeleteDTO::Wrapper& dto,
		const std::shared_ptr<CustomerAuthorizeObject>& authObject
)
{
	// Build unified JSON response
	auto result = StringJsonVO::createShared();

	// Validate required fields
	std::string errMsg = dto->validate();
	if (!errMsg.empty())
	{
		result->init(oatpp::String(errMsg), RS_PARAMS_INVALID);
		return result;
	}

	// Call service to execute admission cancellation
	AdmissionService service;
	bool success = service.deleteAdmission(dto, authObject->getPayload());

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
