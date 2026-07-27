#pragma once
#ifndef _ROOM_VO_H_
#define _ROOM_VO_H_

#include "ApiHelper.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class RoomVO : public oatpp::DTO
{
	DTO_INIT(RoomVO, DTO);
	API_DTO_FIELD(String, id, "Room ID", true, "322955414072725504");
	API_DTO_FIELD(String, name, "Room name", true, "301");
	API_DTO_FIELD(String, busNo, "Business number", true, "LO0028.LO0029");
	API_DTO_FIELD(String, statusEnum, "Status enum", true, "ACTIVE");
	API_DTO_FIELD(String, operationalEnum, "Operational enum", false, "NOT_IN_USE");
	API_DTO_FIELD(String, formEnum, "Location form enum. Room uses HOUSE in adm_location.", true, "HOUSE");
	API_DTO_FIELD(String, modeEnum, "Location mode enum", true, "INSTANCE");
	API_DTO_FIELD(String, typeJson, "Type JSON snapshot", false, "{}");
	API_DTO_FIELD(String, organizationId, "Organization ID", true, "322663308812357632");
	API_DTO_FIELD(Int32, displayOrder, "Display order", false, 1);
	API_DTO_FIELD(String, pyStr, "Pinyin code", false, "301");
	API_DTO_FIELD(String, wbStr, "Wubi code", false, "301");
	API_DTO_FIELD(String, parentId, "Parent ward ID", true, "322955305339588608");
	API_DTO_FIELD(Int32, version, "Optimistic lock version", true, 0);
};

class RoomJsonVO : public JsonVO<RoomVO::Wrapper>
{
	DTO_INIT(RoomJsonVO, JsonVO<RoomVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _ROOM_VO_H_
