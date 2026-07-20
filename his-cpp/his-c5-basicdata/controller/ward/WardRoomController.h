#pragma once
/*
病房病区查询
*/
#ifndef _WARD_CONTROLLER_
#define _WARD_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/ward/WardRoomQuery.h"
#include "domain/vo/ward/WardRommVO.h"

// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) 

#define API_TAG_WARD ZH_WORDS_GETTER("ward.tags")

class WardController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(WardController);

public:
	// 获取病区下的病房列表（根据 wardId 查询），每条记录须包含 id name
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("ward.rooms.summary"), listRooms, WardQuery, ListJsonVO<RoomVO>::Wrapper, API_TAG_WARD);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/ward/rooms", listRooms, WardQuery, execListRooms(query));

	// 获取病房下的病床列表（根据 roomId 查询），每条记录须包含 id name
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("ward.beds.summary"), listBeds, WardQuery, ListJsonVO<BedVO>::Wrapper, API_TAG_WARD);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/ward/beds", listBeds, WardQuery, execListBeds(query));

private:
	ListJsonVO<RoomVO>::Wrapper execListRooms(const WardQuery::Wrapper& query);
	ListJsonVO<BedVO>::Wrapper execListBeds(const WardQuery::Wrapper& query);
};

#undef API_TAG_WARD

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController)

#endif // _WARD_CONTROLLER_