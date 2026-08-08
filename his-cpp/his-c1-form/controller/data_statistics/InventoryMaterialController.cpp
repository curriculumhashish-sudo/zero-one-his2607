#include "stdafx.h"
#include "InventoryMaterialController.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"
#include "service/data_statistics/InventoryMaterialService.h"

InventoryMaterialPageJsonVo::Wrapper InventoryMaterialController::execQueryObject1(const InventoryMaterialQuery::Wrapper& query) {
	// 查询数据
	auto result = InventoryMaterialService().listAll(query);
	// 相应结果
	auto jvo = InventoryMaterialPageJsonVo::createShared();
	jvo->success(result);
	return jvo;
}
