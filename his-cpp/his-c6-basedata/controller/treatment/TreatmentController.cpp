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

namespace
{
	TreatmentDetailDTO::Wrapper buildSampleTreatment(const String& id, const String& code, const String& name)
	{
		auto dto = TreatmentDetailDTO::createShared();
		dto->id = id;
		dto->code = code;
		dto->status = 1;
		dto->name = name;
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
		dto->children = oatpp::List<TreatmentChildDTO::Wrapper>::createShared();
		return dto;
	}
}

TreatmentDetailJsonVO::Wrapper TreatmentController::execQueryOne(const String& id)
{
	// Return sample data until the Service and DAO layers are implemented.
	auto dto = buildSampleTreatment(id, "AD00000001", u8"\u4ee3\u714e\u4e2d\u836f");
	auto vo = TreatmentDetailJsonVO::createShared();
	vo->success(dto);
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execModifyStatus(
	const TreatmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate the request and return a sample success response.
	auto vo = StringJsonVO::createShared();
	if (!dto || !dto->ids || dto->ids->size() == 0) {
		vo->fail("ids empty");
		return vo;
	}
	vo->success("OK");
	return vo;
}

TreatmentSelectPageJsonVO::Wrapper TreatmentController::execQuerySelect(const TreatmentQuery::Wrapper& query)
{
	auto pageDto = TreatmentSelectPageDTO::createShared();

	auto first = TreatmentSelectDTO::createShared();
	first->id = "184000000000000001";
	first->name = u8"\u4ee3\u714e\u4e2d\u836f";
	pageDto->addData(first);

	auto second = TreatmentSelectDTO::createShared();
	second->id = "184000000000000002";
	second->name = u8"\u6ce8\u5c04\u7528\u836f";
	pageDto->addData(second);

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
	pageDto->addData(buildSampleTreatment("184000000000000001", "AD00000001", u8"\u4ee3\u714e\u4e2d\u836f"));

	auto second = buildSampleTreatment("184000000000000002", "AD00000002", u8"\u6ce8\u5c04\u7528\u836f");
	second->categoryCode = "32";
	second->typeEnum = 2;
	second->packageUnitCode = "2";
	second->pyStr = "ZSY";
	second->wbStr = "ZZY";
	second->itemTypeCode = "1014";
	second->purchasePrice = 1.50;
	second->retailPrice = 2.00;
	pageDto->addData(second);

	pageDto->pageIndex = query->pageIndex;
	pageDto->pageSize = query->pageSize;
	pageDto->total = 2;
	pageDto->calcPages();

	auto vo = TreatmentPageJsonVO::createShared();
	vo->success(pageDto);
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execAddTreatment(
	const TreatmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Return a fabricated identifier in this sample implementation.
	auto vo = StringJsonVO::createShared();
	vo->success("184000000000000002");
	return vo;
}

StringJsonVO::Wrapper TreatmentController::execModifyTreatment(
	const TreatmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Echo the record identifier in this sample implementation.
	auto vo = StringJsonVO::createShared();
	vo->success(dto->id);
	return vo;
}
