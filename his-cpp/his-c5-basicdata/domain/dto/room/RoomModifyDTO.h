#pragma once
#ifndef _ROOM_MODIFY_DTO_H_
#define _ROOM_MODIFY_DTO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class RoomModifyDTO : public oatpp::DTO
{
	DTO_INIT(RoomModifyDTO, DTO);
	API_DTO_FIELD(String, id, "Room ID", true, "322955414072725504");
	API_DTO_FIELD(String, name, "Room name", true, "301");
	API_DTO_FIELD(String, parentId, "Parent ward ID", true, "322955305339588608");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, pyStr, "Pinyin code", false, "301");
	API_DTO_FIELD(String, wbStr, "Wubi code", false, "301");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _ROOM_MODIFY_DTO_H_
