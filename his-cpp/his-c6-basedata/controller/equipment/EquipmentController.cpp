#include "stdafx.h"
#include "EquipmentController.h"
#include "../../service/equipment/EquipmentService.h"
#include "../../domain/bo/PdfReportBO.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"

// 器材列表分页查询
EquipmentDetailPageJsonVO::Wrapper EquipmentController::executeQueryEquipment(const EquipmentQuery::Wrapper& query)
{
	// 查询数据
	auto result = EquipmentService().listAll(query);
	// 响应结果
	auto jvo = EquipmentDetailPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

// 新增器材
StringJsonVO::Wrapper EquipmentController::execAddEquipment(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return{};
}

//修改设备
StringJsonVO::Wrapper EquipmentController::execModifyEquipment(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//批量批量删除器材
StringJsonVO::Wrapper EquipmentController::execDeleteEquipment(const EquipmentDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//批量启停用器材
StringJsonVO::Wrapper EquipmentController::execChangeEquipmentStatus(const EquipmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
}

//选择器材
EquipmentSelectPageJsonVO::Wrapper EquipmentController::execSelectEquipment(const EquipmentQuery::Wrapper& query)
{
	return {};
}

EquipmentDetailJsonVO::Wrapper EquipmentController::executeQueryOne(const String& id)
{
	return {};
}