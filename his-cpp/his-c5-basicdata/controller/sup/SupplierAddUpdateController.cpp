#include "stdafx.h"
#include "SupplierAddUpdateController.h"

SupplierAddJsonVO::Wrapper SupplierAddUpdateController::execAddSupplier(const SupplierAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = SupplierAddJsonVO::createShared();
	vo->init(String(""), RS_API_UN_IMPL);
	return vo;
}

SupplierUpdateJsonVO::Wrapper SupplierAddUpdateController::execModifySupplier(const SupplierUpdateDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = SupplierUpdateJsonVO::createShared();
	vo->init(RS_API_UN_IMPL);
	return vo;
}