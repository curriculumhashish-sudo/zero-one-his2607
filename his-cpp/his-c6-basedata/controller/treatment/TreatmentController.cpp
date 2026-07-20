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
