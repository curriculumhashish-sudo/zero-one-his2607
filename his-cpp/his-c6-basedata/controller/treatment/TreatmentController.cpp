/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/19 00:00:00

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
#include "TreatmentController.h"

TreatmentDetailJsonVO::Wrapper TreatmentController::execQueryOne(const String& id)
{
	// This stage only defines the domain model and Swagger contract; no Service/DAO call yet.
	// The example data below mirrors the "Edit Treatment" dialog fields for id=184000000000000001.
	auto dto = TreatmentDetailDTO::createShared();
	dto->id = id;
	dto->code = "AD00000001";
	dto->status = 1;

	dto->name = u8"代煎中药";
	dto->categoryCode = "31";
	dto->typeEnum = 5;
	dto->packageUnitCode = "6";
	dto->pyStr = "DJZY";
	dto->wbStr = "WUKA";
	dto->itemTypeCode = "1013";
	dto->purchasePrice = 3.00;
	dto->retailPrice = 3.00;
	dto->maximumRetailPrice = nullptr;
	dto->descriptionText = "";

	dto->ybFlag = nullptr;
	dto->ybNo = nullptr;
	dto->ybType = nullptr;
	dto->ybMatchFlag = nullptr;
	dto->chrgitmLv = nullptr;

	// This record has no sub-items configured, matching the empty "Sub-items" row in the prototype.
	dto->children = oatpp::List<TreatmentChildDTO::Wrapper>::createShared();

	auto vo = TreatmentDetailJsonVO::createShared();
	vo->success(dto);
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execModifyStatus(const TreatmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 示例实现：只做简单校验并返回成功。真实实现应调用 Service 层并处理失败情况。
	auto vo = StringJsonVO::createShared();
	if (!dto || !dto->ids || dto->ids->size() == 0) {
		vo->fail("ids empty");
		return vo;
	}
	// pretend we updated statuses
	vo->success(u8"OK");
	return vo;
}

TreatmentSelectPageJsonVO::Wrapper TreatmentController::execQuerySelect(const TreatmentQuery::Wrapper& query)
{
	auto pageDto = TreatmentSelectPageDTO::createShared();

	// sample selectable rows
	auto r1 = TreatmentSelectDTO::createShared();
	r1->id = "184000000000000001";
	r1->name = u8"代煎中药";
	pageDto->addData(r1);

	auto r2 = TreatmentSelectDTO::createShared();
	r2->id = "184000000000000002";
	r2->name = u8"注射用药";
	pageDto->addData(r2);

	pageDto->pageIndex = query->pageIndex;
	pageDto->pageSize = query->pageSize;
	pageDto->total = 2;
	pageDto->calcPages();

	auto vo = TreatmentSelectPageJsonVO::createShared();
	vo->success(pageDto);
	return vo;
}

TreatmentPageJsonVO::Wrapper TreatmentController::execQueryAll(const TreatmentQuery::Wrapper& query)
{
	auto pageDto = TreatmentPageDTO::createShared();

	// sample detailed rows
	auto d1 = TreatmentDetailDTO::createShared();
	d1->id = "184000000000000001";
	d1->code = "AD00000001";
	d1->status = 1;
	d1->name = u8"代煎中药";
	d1->categoryCode = "31";
	d1->typeEnum = 5;
	d1->packageUnitCode = "6";
	d1->pyStr = "DJZY";
	d1->wbStr = "WUKA";
	d1->itemTypeCode = "1013";
	d1->purchasePrice = 3.00;
	d1->retailPrice = 3.00;
	d1->maximumRetailPrice = nullptr;
	d1->descriptionText = "";
	d1->ybFlag = nullptr;
	d1->ybNo = nullptr;
	d1->ybType = nullptr;
	d1->ybMatchFlag = nullptr;
	d1->chrgitmLv = nullptr;
	d1->children = oatpp::List<TreatmentChildDTO::Wrapper>::createShared();
	pageDto->addData(d1);

	auto d2 = TreatmentDetailDTO::createShared();
	d2->id = "184000000000000002";
	d2->code = "AD00000002";
	d2->status = 1;
	d2->name = u8"注射用药";
	d2->categoryCode = "32";
	d2->typeEnum = 2;
	d2->packageUnitCode = "2";
	d2->pyStr = "ZSY";
	d2->wbStr = "ZZY";
	d2->itemTypeCode = "1014";
	d2->purchasePrice = 1.50;
	d2->retailPrice = 2.00;
	d2->maximumRetailPrice = nullptr;
	d2->descriptionText = "";
	d2->ybFlag = nullptr;
	d2->ybNo = nullptr;
	d2->ybType = nullptr;
	d2->ybMatchFlag = nullptr;
	d2->chrgitmLv = nullptr;
	d2->children = oatpp::List<TreatmentChildDTO::Wrapper>::createShared();
	pageDto->addData(d2);

	pageDto->pageIndex = query->pageIndex;
	pageDto->pageSize = query->pageSize;
	pageDto->total = 2;
	pageDto->calcPages();

	auto vo = TreatmentPageJsonVO::createShared();
	vo->success(pageDto);
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execAddTreatment(const TreatmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// This stage only defines the domain model and Swagger contract; no Service/DAO call yet.
	// The example response below returns a fabricated new record id.
	auto vo = StringJsonVO::createShared();
	vo->success("184000000000000002");
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execModifyTreatment(const TreatmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// This stage only defines the domain model and Swagger contract; no Service/DAO call yet.
	// The example response below echoes back the modified record id.
	auto vo = StringJsonVO::createShared();
	vo->success(dto->id);
	return vo;
}
