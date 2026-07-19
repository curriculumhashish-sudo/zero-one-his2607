#pragma once

#ifndef _RENTCONTROLLER_H_
#define _RENTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/equipment/EquipmentQuery.h"
#include "domain/vo/equipment/EquipmentVO.h"
// #include "domain/dto/equipment/EquipmentModifyDTO.h"
#include "domain/dto/equipment/EquipmentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("equipment.tags.tag")

class EquipmentController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(EquipmentController);
public: // 定义接口
	// 3.1 定义分页查询租户接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("equipment.query.summary"), queryEquipment, EquipmentQuery, EquipmentDetailPageJsonVO::Wrapper, API_TAG);
	// 3.2 定义分页查询租户接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/equipment/query", queryEquipment, EquipmentQuery, executeQueryEquipment(query));

	// 3.1 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.add.summary"), addEquipment, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/equipment/add", addEquipment, BODY_DTO(EquipmentAddDTO::Wrapper, dto), execAddEquipment(dto, authObject->getPayload()));

private: // 定义接口执行函数
	// 3.3 定义分页查询租户接口执行函数
	EquipmentDetailPageJsonVO::Wrapper executeQueryEquipment(const EquipmentQuery::Wrapper& query);
	// 3.3 定义新增接口执行函数
	StringJsonVO::Wrapper execAddEquipment(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload);
	
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_RENTCONTROLLER_H_