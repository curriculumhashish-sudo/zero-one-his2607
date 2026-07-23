#pragma once
#ifndef _ROOM_CONTROLLER_H_
#define _ROOM_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/dto/room/RoomModifyDTO.h"
#include "domain/dto/room/RoomStatusDTO.h"
#include "domain/vo/room/RoomVO.h"
#include "domain/vo/BaseJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG "Room Management"

class RoomController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(RoomController);

public:
	API_DEF_ENDPOINT_INFO_AUTH("Modify room", modifyRoom, RoomJsonVO::Wrapper, API_TAG,
		info->description = "Modify room basic information by ID. This endpoint returns mock success data for API documentation.";
		info->addConsumes<RoomModifyDTO::Wrapper>("application/json");
	)
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/room", modifyRoom, BODY_DTO(RoomModifyDTO::Wrapper, dto), execModifyRoom(dto));

	API_DEF_ENDPOINT_INFO_AUTH("Remove room", removeRoom, BooleanJsonVO::Wrapper, API_TAG,
		info->description = "Remove room by ID. This endpoint returns a mock success result for API documentation.";
		API_DEF_ADD_PATH_PARAMS(String, "id", "Room ID", "322955414072725504", true);
	)
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/room/{id}", removeRoom, PATH(String, id, "id"), execRemoveRoom(id));

	API_DEF_ENDPOINT_INFO_AUTH("Change room status", modifyRoomStatus, RoomJsonVO::Wrapper, API_TAG,
		info->description = "Enable or disable room by statusEnum. ACTIVE means enabled and DISABLED means disabled.";
		info->addConsumes<RoomStatusDTO::Wrapper>("application/json");
	)
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/room/status", modifyRoomStatus, BODY_DTO(RoomStatusDTO::Wrapper, dto), execModifyRoomStatus(dto));

private:
	RoomJsonVO::Wrapper execModifyRoom(const RoomModifyDTO::Wrapper& dto)
	{
		auto data = RoomVO::createShared();
		if (dto)
		{
			data->id = dto->id;
			data->name = dto->name;
			data->parentId = dto->parentId;
			data->organizationId = dto->organizationId;
			data->displayOrder = dto->displayOrder;
			data->pyStr = dto->pyStr;
			data->wbStr = dto->wbStr;
			data->version = dto->version;
		}
		auto result = RoomJsonVO::createShared();
		result->success(data);
		return result;
	}

	BooleanJsonVO::Wrapper execRemoveRoom(const String& id)
	{
		auto result = BooleanJsonVO::createShared();
		result->success(true);
		return result;
	}

	RoomJsonVO::Wrapper execModifyRoomStatus(const RoomStatusDTO::Wrapper& dto)
	{
		auto data = RoomVO::createShared();
		if (dto)
		{
			data->id = dto->id;
			data->statusEnum = dto->statusEnum;
			data->version = dto->version;
		}
		auto result = RoomJsonVO::createShared();
		result->success(data);
		return result;
	}
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _ROOM_CONTROLLER_H_
