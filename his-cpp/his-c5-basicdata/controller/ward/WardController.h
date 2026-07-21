#pragma once
/*
病房病区查询
*/
#ifndef _WARD_CONTROLLER_
#define _WARD_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/ward/WardQuery.h"
#include "domain/vo/ward/WardVO.h"
#include "domain/dto/ward/WardDTO.h"

// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) 

#define API_TAG ZH_WORDS_GETTER("ward.tags")

class WardController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(WardController);

public:
	// 3.1 定义ID查询接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("ward.query-room-by-id.summary"), queryRoomById, WardRoomJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("ward.field.roomId"), "2233", true);
	);
	// 3.2 定义ID查询接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/patient/query-room-by-id", queryRoomById, QUERY(String, id), execQueryRoomById(id));

	// 3.1 定义ID查询接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("ward.query-bed-by-id.summary"), queryBedById, WardBedJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("ward.field.bedId"), "2233", true);
	);
	// 3.2 定义ID查询接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/ward/query-bed-by-id", queryBedById, QUERY(String, id), execQueryBedById(id));
private:
	// 3.3 演示ID查询数据
	WardRoomJsonVO::Wrapper execQueryRoomById(const String& id);
	// 3.3 演示ID查询数据
	WardBedJsonVO::Wrapper execQueryBedById(const String& id);
};

#undef API_TAG_WARD

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController)

#endif // _WARD_CONTROLLER_