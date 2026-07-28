#include "stdafx.h"
#include "InventoryMaterialController.h"
#include "../ApiDeclarativeServicesHelper.h"
#include "ExcelComponent.h"
#include "SimpleDateTimeFormat.h"
#include "PdfComponent.h"
#include "service/data_statistics/InventoryMaterialService.h"

InventoryMaterialPageDto::Wrapper InventoryMaterialController::execQueryObject1(const InventoryMaterialQuery::Wrapper& query) {
	// 查询数据
	auto result = InventoryMaterialService().listAll(query);
	// 相应结果
	auto jvo = InventoryMaterialPageJsonVo::createShared();
	jvo->success(result);
	return jvo;
}

InventoryMaterialVo::Wrapper InventoryMaterialController::execQueryById(const String& id) {
	auto jvo = InventoryMaterialVo::createShared();

	if (!id) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据新增
	auto result = InventoryMaterialService().getById(id.getValue({}));
	jvo->success(result);
	return jvo;
}
