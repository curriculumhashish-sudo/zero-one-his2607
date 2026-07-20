#ifndef _SUPPLIER_ADD_UPDATE_CONTROLLER_H_
#define _SUPPLIER_ADD_UPDATE_CONTROLLER_H_

#include "domain/dto/sup/SupplierAddDTO.h"
#include "domain/dto/sup/SupplierUpdateDTO.h"
#include "domain/vo/sup/SupplierAddUpdateVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG "Supplier Add and Update"

class SupplierAddUpdateController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(SupplierAddUpdateController);

public:
	// add supplier
	API_DEF_ENDPOINT_INFO_AUTH(
		"Add supplier",
		addSupplier,
		SupplierAddJsonVO::Wrapper,
		API_TAG
	);

	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/supplier/add-supplier",
		addSupplier,
		BODY_DTO(SupplierAddDTO::Wrapper, dto),
		execAddSupplier(dto, authObject->getPayload())
	);

	// modify supplier
	API_DEF_ENDPOINT_INFO_AUTH(
		"Modify supplier",
		modifySupplier,
		SupplierUpdateJsonVO::Wrapper,
		API_TAG
	);

	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/supplier/modify-supplier",
		modifySupplier,
		BODY_DTO(SupplierUpdateDTO::Wrapper, dto),
		execModifySupplier(dto, authObject->getPayload())
	);

private:
	SupplierAddJsonVO::Wrapper execAddSupplier(const SupplierAddDTO::Wrapper& dto, const PayloadDTO& payload);
	SupplierUpdateJsonVO::Wrapper execModifySupplier(const SupplierUpdateDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_SUPPLIER_ADD_UPDATE_CONTROLLER_H_
