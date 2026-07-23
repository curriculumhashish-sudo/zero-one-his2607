#pragma once
#ifndef _BED_CONTROLLER_H_
#define _BED_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/dto/bed/BedModifyDTO.h"
#include "domain/dto/bed/BedStatusDTO.h"
#include "domain/vo/bed/BedVO.h"
#include "domain/vo/BaseJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG "Bed Management"

class BedController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(BedController);

public:
	API_DEF_ENDPOINT_INFO_AUTH("Modify bed", modifyBed, BedJsonVO::Wrapper, API_TAG,
		info->description = "Modify bed basic information by ID. This endpoint returns mock success data for API documentation.";
		info->addConsumes<BedModifyDTO::Wrapper>("application/json");
	)
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/bed", modifyBed, BODY_DTO(BedModifyDTO::Wrapper, dto), execModifyBed(dto));

	API_DEF_ENDPOINT_INFO_AUTH("Remove bed", removeBed, BooleanJsonVO::Wrapper, API_TAG,
		info->description = "Remove bed by ID. This endpoint returns a mock success result for API documentation.";
		API_DEF_ADD_PATH_PARAMS(String, "id", "Bed ID", "322955479021522944", true);
	)
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/bed/{id}", removeBed, PATH(String, id, "id"), execRemoveBed(id));

	API_DEF_ENDPOINT_INFO_AUTH("Change bed status", modifyBedStatus, BedJsonVO::Wrapper, API_TAG,
		info->description = "Enable or disable bed by statusEnum. ACTIVE means enabled and DISABLED means disabled.";
		info->addConsumes<BedStatusDTO::Wrapper>("application/json");
	)
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/bed/status", modifyBedStatus, BODY_DTO(BedStatusDTO::Wrapper, dto), execModifyBedStatus(dto));

private:
	BedJsonVO::Wrapper execModifyBed(const BedModifyDTO::Wrapper& dto)
	{
		auto data = BedVO::createShared();
		if (dto)
		{
			data->id = dto->id;
			data->name = dto->name;
			data->parentId = dto->parentId;
			data->organizationId = dto->organizationId;
			data->displayOrder = dto->displayOrder;
			data->typeJson = dto->typeJson;
			data->version = dto->version;
		}
		auto result = BedJsonVO::createShared();
		result->success(data);
		return result;
	}

	BooleanJsonVO::Wrapper execRemoveBed(const String& id)
	{
		auto result = BooleanJsonVO::createShared();
		result->success(true);
		return result;
	}

	BedJsonVO::Wrapper execModifyBedStatus(const BedStatusDTO::Wrapper& dto)
	{
		auto data = BedVO::createShared();
		if (dto)
		{
			data->id = dto->id;
			data->statusEnum = dto->statusEnum;
			data->version = dto->version;
		}
		auto result = BedJsonVO::createShared();
		result->success(data);
		return result;
	}
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _BED_CONTROLLER_H_
