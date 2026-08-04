#include "stdafx.h"
#include "SupplierAddUpdateController.h"

#include "domain/sup/SupplierStatus.h"
#include "service/sup/SupplierService.h"

SupplierAddJsonVO::Wrapper SupplierAddUpdateController::execAddSupplier(const SupplierAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = SupplierAddJsonVO::createShared();
	// Reject a missing request body.
	if (!dto)
	{
		vo->init(oatpp::String(nullptr), SupplierStatus::invalid("request body is required."));
		return vo;
	}
	// Field-level validation performed by the DTO.
	const std::string error = dto->validate();
	if (!error.empty())
	{
		vo->init(oatpp::String(nullptr), SupplierStatus::invalid(error));
		return vo;
	}

	// Hand the authenticated user to the service layer.
	dto->setPayload(&payload);
	auto result = SupplierService().add(dto);

	// On success the VO carries the new supplier id; on failure data is null.
	vo->init(
		result.success ? oatpp::String(result.id.c_str()) : oatpp::String(nullptr),
		result.status);
	return vo;
}

SupplierUpdateJsonVO::Wrapper SupplierAddUpdateController::execModifySupplier(const SupplierUpdateDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = SupplierUpdateJsonVO::createShared();
	// Reject a missing request body.
	if (!dto)
	{
		vo->init(SupplierStatus::invalid("request body is required."));
		return vo;
	}
	// Field-level validation performed by the DTO.
	const std::string error = dto->validate();
	if (!error.empty())
	{
		vo->init(SupplierStatus::invalid(error));
		return vo;
	}

	// Hand the authenticated user to the service layer.
	dto->setPayload(&payload);
	auto result = SupplierService().modify(dto);

	// NoDataJsonVO carries only the business code and message.
	vo->init(result.status);
	return vo;
}