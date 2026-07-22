#include "stdafx.h"
#include "EquipmentController.h"

EquipmentDetailPageJsonVO::Wrapper EquipmentController::executeQueryEquipment(const EquipmentQuery::Wrapper& query)
{
	return {};
}

StringJsonVO::Wrapper EquipmentController::execAddEquipment(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return {};
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

EquipmentDetailJsonVO::Wrapper EquipmentController::executeQueryOne(const String& id)
{
	return {};
}