/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "controller/data_statistics/InventoryMaterialController.h"
#include "ApiHelper.h"
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