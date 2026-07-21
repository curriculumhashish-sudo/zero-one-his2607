#pragma once

#ifndef _RENTCONTROLLER_H_
#define _RENTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/equipment/EquipmentQuery.h"
#include "domain/vo/equipment/EquipmentVO.h"
#include "domain/dto/equipment/EquipmentAddDTO.h"
#include "domain/dto/equipment/EquipmentModifyDTO.h"
#include "domain/dto/equipment/EquipmentDeleteDTO.h"
#include "domain/dto/equipment/EquipmentStatusDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("equipment.tags.tag")

class EquipmentController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(EquipmentController);
public: // 定义接口
	// 3.1 定义分页查询器材接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("equipment.query.summary"), queryEquipment, EquipmentQuery, EquipmentDetailPageJsonVO::Wrapper, API_TAG);
	// 3.2 定义分页查询器材接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/equipment/query", queryEquipment, EquipmentQuery, executeQueryEquipment(query));

	// 3.1 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.add.summary"), addEquipment, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/equipment/add", addEquipment, BODY_DTO(EquipmentAddDTO::Wrapper, dto), execAddEquipment(dto, authObject->getPayload()));

	// 3.1 定义修改器材接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.modify.summary"), modifyEquipment, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改器材接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/equipment/modify", modifyEquipment, BODY_DTO(EquipmentModifyDTO::Wrapper, dto), execModifyEquipment(dto, authObject->getPayload()));

	// 3.1 定义批量删除器材接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.delete.summary"), deleteEquipment, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义批量删除器材接口描述
	API_HANDLER_ENDPOINT_AUTH(API_M_DELETE, "/equipment/delete", deleteEquipment, BODY_DTO(EquipmentDeleteDTO::Wrapper, dto), execDeleteEquipment(dto, authObject->getPayload()));

	// 3.1 定义批量启停用器材接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.status.summary"), changeEquipmentStatus, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义批量启停用器材接口描述
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/equipment/status", changeEquipmentStatus, BODY_DTO(EquipmentStatusDTO::Wrapper, dto), execChangeEquipmentStatus(dto, authObject->getPayload()));

	// 定义获取用户详细信息接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.query-one.summary"), queryOneEquipment, EquipmentDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("equipment.field.id"), "ae65c714d48d4f34b52479f5482c0edd", true);
	);
	// 定义获取用户详细信息接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/equipment/query-one", queryOneEquipment, QUERY(String, id), executeQueryOne(id));
private: // 定义接口执行函数
	// 3.3 定义分页查询器材接口执行函数
	EquipmentDetailPageJsonVO::Wrapper executeQueryEquipment(const EquipmentQuery::Wrapper& query);
	// 3.3 定义新增接口执行函数
	StringJsonVO::Wrapper execAddEquipment(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 3.3 定义修改器材接口执行函数
	StringJsonVO::Wrapper execModifyEquipment(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload);
	// 3.3 定义批量删除器材接口执行函数
	StringJsonVO::Wrapper execDeleteEquipment(const EquipmentDeleteDTO::Wrapper& dto, const PayloadDTO& payload);
	// 3.3 定义批量启停用器材接口执行函数
	StringJsonVO::Wrapper execChangeEquipmentStatus(const EquipmentStatusDTO::Wrapper& dto, const PayloadDTO& payload);
	
	// 查询详情
	EquipmentDetailJsonVO::Wrapper executeQueryOne(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_RENTCONTROLLER_H_