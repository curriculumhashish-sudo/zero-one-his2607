#pragma once
#ifndef _ROOM_STATUS_DTO_H_
#define _ROOM_STATUS_DTO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class RoomStatusDTO : public oatpp::DTO
{
	DTO_INIT(RoomStatusDTO, DTO);
	API_DTO_FIELD(String, id, "Room ID", true, "322955414072725504");
	API_DTO_FIELD(String, statusEnum, "Status enum: ACTIVE or DISABLED", true, "ACTIVE");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _ROOM_STATUS_DTO_H_
