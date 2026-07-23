#pragma once
#ifndef _WARD_CONTROLLER_H_
#define _WARD_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/dto/ward/WardModifyDTO.h"
#include "domain/dto/ward/WardStatusDTO.h"
#include "domain/vo/ward/WardVO.h"
#include "domain/vo/BaseJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG "Ward Management"

class WardController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(WardController);

public:
	API_DEF_ENDPOINT_INFO("Modify ward", modifyWard, WardJsonVO::Wrapper, API_TAG,
		info->description = "Modify ward basic information by ID. This endpoint returns mock success data for API documentation.";
		info->headers.add<String>(API_H_TOKEN).description = "Authorization token";
		info->headers[API_H_TOKEN].addExample("default", String("Bearer test"));
		info->headers[API_H_TOKEN].required = true;
		info->addConsumes<WardModifyDTO::Wrapper>("application/json");
	)
	ENDPOINT(API_M_PUT, "/ward", modifyWard, BODY_DTO(WardModifyDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execModifyWard(dto));
	}

	API_DEF_ENDPOINT_INFO("Remove ward", removeWard, BooleanJsonVO::Wrapper, API_TAG,
		info->description = "Remove ward by ID. This endpoint returns a mock success result for API documentation.";
		info->headers.add<String>(API_H_TOKEN).description = "Authorization token";
		info->headers[API_H_TOKEN].addExample("default", String("Bearer test"));
		info->headers[API_H_TOKEN].required = true;
		API_DEF_ADD_PATH_PARAMS(String, "id", "Ward ID", "322955305339588608", true);
	)
	ENDPOINT(API_M_DEL, "/ward/{id}", removeWard, PATH(String, id, "id")) {
		API_HANDLER_RESP_VO(execRemoveWard(id));
	}

	API_DEF_ENDPOINT_INFO("Change ward status", modifyWardStatus, WardJsonVO::Wrapper, API_TAG,
		info->description = "Enable or disable ward by statusEnum. ACTIVE means enabled and DISABLED means disabled.";
		info->headers.add<String>(API_H_TOKEN).description = "Authorization token";
		info->headers[API_H_TOKEN].addExample("default", String("Bearer test"));
		info->headers[API_H_TOKEN].required = true;
		info->addConsumes<WardStatusDTO::Wrapper>("application/json");
	)
	ENDPOINT(API_M_PUT, "/ward/status", modifyWardStatus, BODY_DTO(WardStatusDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execModifyWardStatus(dto));
	}

private:
	WardJsonVO::Wrapper execModifyWard(const WardModifyDTO::Wrapper& dto)
	{
		auto data = WardVO::createShared();
		if (dto)
		{
			data->id = dto->id;
			data->name = dto->name;
			data->organizationId = dto->organizationId;
			data->displayOrder = dto->displayOrder;
			data->pyStr = dto->pyStr;
			data->wbStr = dto->wbStr;
			data->version = dto->version;
		}
		auto result = WardJsonVO::createShared();
		result->success(data);
		return result;
	}

	BooleanJsonVO::Wrapper execRemoveWard(const String& id)
	{
		auto result = BooleanJsonVO::createShared();
		result->success(true);
		return result;
	}

	WardJsonVO::Wrapper execModifyWardStatus(const WardStatusDTO::Wrapper& dto)
	{
		auto data = WardVO::createShared();
		if (dto)
		{
			data->id = dto->id;
			data->statusEnum = dto->statusEnum;
			data->version = dto->version;
		}
		auto result = WardJsonVO::createShared();
		result->success(data);
		return result;
	}
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _WARD_CONTROLLER_H_
