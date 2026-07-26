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
#include "domain/dto/equipment/EquipmentSelectDTO.h"

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
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.remove.summary"), deleteEquipment, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义批量删除器材接口描述
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/equipment/delete", deleteEquipment, BODY_DTO(EquipmentDeleteDTO::Wrapper, dto), execDeleteEquipment(dto, authObject->getPayload()));

	// 3.1 定义批量启停用器材接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("equipment.status.summary"), changeEquipmentStatus, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义批量启停用器材接口描述
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/equipment/status", changeEquipmentStatus, BODY_DTO(EquipmentStatusDTO::Wrapper, dto), execChangeEquipmentStatus(dto, authObject->getPayload()));

	// 3.1 定义选择器材接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("equipment.select.summary"), selectEquipment, EquipmentQuery, EquipmentSelectPageJsonVO::Wrapper, API_TAG);
	// 3.2 定义选择器材接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/equipment/select", selectEquipment, EquipmentQuery, execSelectEquipment(query));

	// 3.1 定义ID查询接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("equipment.query-by-id.summary"), EquipmentById, EquipmentDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("sample.field.id"), "d934050a8bb373e8f8eed0bf7507ec17", true);
	);
	// 3.2 定义ID查询接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/sample/query-by-id", querySampleById, QUERY(String, id), execQueryById(id));

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
	// 3.3 定义选择器材执行函数
	EquipmentSelectPageJsonVO::Wrapper execSelectEquipment(const EquipmentQuery::Wrapper& query);
	// 3.3 演示ID查询数据
	EquipmentJsonVO::Wrapper execQueryById(const String& id);
	// 3.3 测试声明式服务调用1
	EquipmentJsonVO::Wrapper execQueryOne(const String& id, const PayloadDTO& payload);
	// 3.3 测试声明式服务调用2
	EquipmentPageJsonVO::Wrapper execQueryAll(const EquipmentQuery::Wrapper& query, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_RENTCONTROLLER_H_