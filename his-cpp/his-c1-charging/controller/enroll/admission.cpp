#include "stdafx.h"
#include "admission.h"
#include "../../service/enroll/AdmissionService.h"

// Build a failure ResultStatus for a DAO/Service error code.
// Business failures use code 9999; unknown codes fall back to 9994.
static ResultStatus buildErrorStatus(const std::string& code)
{
	if (code == "DATE_INVALID")
		return ResultStatus(ZH_WORDS_GETTER("charging.admission.error.date_invalid"), 9999);

	if (code == "ENCOUNTER_NOT_FOUND")
		return ResultStatus(ZH_WORDS_GETTER("charging.admission.error.encounter_not_found"), 9999);

	if (code == "STATUS_NOT_CANCELABLE")
		return ResultStatus(ZH_WORDS_GETTER("charging.admission.error.status_not_cancelable"), 9999);

	if (code == "INSERT_FAILED" || code == "DELETE_FAILED")
		return ResultStatus(ZH_WORDS_GETTER("charging.admission.error.operation_failed"), 9999);

	// Unknown failure: server error
	return RS_SERVER_ERROR;
}

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
	std::string code = service.insertAdmission(dto, authObject->getPayload());

	if (code.empty())
	{
		result->success(oatpp::String(ZH_WORDS_GETTER("charging.admission.add_success")));
	}
	else
	{
		result->init(nullptr, buildErrorStatus(code));
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
	std::string code = service.deleteAdmission(dto, authObject->getPayload());

	if (code.empty())
	{
		result->success(oatpp::String(ZH_WORDS_GETTER("charging.admission.delete_success")));
	}
	else
	{
		result->init(nullptr, buildErrorStatus(code));
	}
	return result;
}
