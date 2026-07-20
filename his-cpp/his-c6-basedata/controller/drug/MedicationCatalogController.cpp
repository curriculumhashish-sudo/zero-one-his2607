/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/20

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
#include "MedicationCatalogController.h"

namespace
{
	bool hasText(const oatpp::String& value)
	{
		return value && !value->empty();
	}

	template<class T>
	bool hasRequiredMedicationFields(const T& dto)
	{
		return dto
			&& hasText(dto->name)
			&& hasText(dto->purchaseUnitCode)
			&& hasText(dto->splitUnitCode)
			&& hasText(dto->doseUnitCode)
			&& hasText(dto->packageUnitCode)
			&& dto->doseUnitRatio != nullptr
			&& dto->purchaseUnitRatio != nullptr
			&& dto->stockUnitRatio != nullptr
			&& dto->packageUnitRatio != nullptr
			&& hasText(dto->statusEnum)
			&& dto->activeFlag != nullptr;
	}

	template<class T>
	void applyMedicationFields(const T& dto, const MedicationCatalogDetailVO::Wrapper& detail)
	{
		if (!dto)
		{
			return;
		}
		detail->tenantId = dto->tenantId;
		detail->orgId = dto->orgId;
		detail->busNo = dto->busNo;
		detail->name = dto->name;
		detail->domainEnum = dto->domainEnum;
		detail->medVersion = dto->medVersion;
		detail->nameEn = dto->nameEn;
		detail->pyStr = dto->pyStr;
		detail->wbStr = dto->wbStr;
		detail->categoryCode = dto->categoryCode;
		detail->merchandiseName = dto->merchandiseName;
		detail->merchandisePyStr = dto->merchandisePyStr;
		detail->merchandiseWbStr = dto->merchandiseWbStr;
		detail->purchaseUnitCode = dto->purchaseUnitCode;
		detail->splitUnitCode = dto->splitUnitCode;
		detail->comprisedText = dto->comprisedText;
		detail->ingredient = dto->ingredient;
		detail->doseFrom = dto->doseFrom;
		detail->approvalNumber = dto->approvalNumber;
		detail->ybMatchFlag = dto->ybMatchFlag;
		detail->ybNo = dto->ybNo;
		detail->pharmacologyCategoryCode = dto->pharmacologyCategoryCode;
		detail->skinTestFlag = dto->skinTestFlag;
		detail->injectFlag = dto->injectFlag;
		detail->manufacturerId = dto->manufacturerId;
		detail->doseUnitCode = dto->doseUnitCode;
		detail->packageUnitCode = dto->packageUnitCode;
		detail->stockUnitCode = dto->stockUnitCode;
		detail->manufacturerText = dto->manufacturerText;
		detail->supplyId = dto->supplyId;
		detail->restrictedFlag = dto->restrictedFlag;
		detail->restrictedScope = dto->restrictedScope;
		detail->childrenFlag = dto->childrenFlag;
		detail->characteristic = dto->characteristic;
		detail->itemMinQuantity = dto->itemMinQuantity;
		detail->itemMaxQuantity = dto->itemMaxQuantity;
		detail->defEncounterUnitCode = dto->defEncounterUnitCode;
		detail->defInhospitalUnitCode = dto->defInhospitalUnitCode;
		detail->nationalDrugCode = dto->nationalDrugCode;
		detail->partAttributeEnum = dto->partAttributeEnum;
		detail->antibioticCode = dto->antibioticCode;
		detail->restrictedEnum = dto->restrictedEnum;
		detail->selfFlag = dto->selfFlag;
		detail->antibioticFlag = dto->antibioticFlag;
		detail->basicFlag = dto->basicFlag;
		detail->thoPartAttributeEnum = dto->thoPartAttributeEnum;
		detail->chrgitmLv = dto->chrgitmLv;
		detail->rxFlag = dto->rxFlag;
		detail->dosageInstruction = dto->dosageInstruction;
		detail->drug69Code = dto->drug69Code;
		detail->totalVolume = dto->totalVolume;
		detail->doseFormCode = dto->doseFormCode;
		detail->methodCode = dto->methodCode;
		detail->rateCode = dto->rateCode;
		detail->dose = dto->dose;
		detail->maxUnit = dto->maxUnit;
		detail->usageLimit = dto->usageLimit;
		detail->locationId = dto->locationId;
		detail->ingredientItem = dto->ingredientItem;
		detail->definition = dto->definition;
		detail->dddCode = dto->dddCode;
		detail->dddUnitCode = dto->dddUnitCode;
		detail->minRateCode = dto->minRateCode;
		detail->maxRateCode = dto->maxRateCode;
		detail->doseUnitRatio = dto->doseUnitRatio;
		detail->purchaseUnitRatio = dto->purchaseUnitRatio;
		detail->stockUnitRatio = dto->stockUnitRatio;
		detail->packageUnitRatio = dto->packageUnitRatio;
		detail->statusEnum = dto->statusEnum;
		detail->activeFlag = dto->activeFlag;
		detail->purchasePrice = dto->purchasePrice;
		detail->retailPrice = dto->retailPrice;
		detail->maximumRetailPrice = dto->maximumRetailPrice;
	}

	NoDataJsonVO::Wrapper createBatchResult(const oatpp::List<oatpp::String>& ids)
	{
		auto result = NoDataJsonVO::createShared();
		if (!ids || ids->empty())
		{
			result->init(RS_PARAMS_INVALID);
			return result;
		}
		for (const auto& id : *ids.get())
		{
			if (!hasText(id))
			{
				result->init(RS_PARAMS_INVALID);
				return result;
			}
		}
		result->init(RS_SUCCESS);
		return result;
	}
}

MedicationCatalogPageJsonVO::Wrapper MedicationCatalogController::execQueryMedicationCatalogPage(
	const MedicationCatalogPageQuery::Wrapper& query)
{
	auto result = MedicationCatalogPageJsonVO::createShared();
	if (!query || query->pageNo.getValue(0) <= 0 || query->pageSize.getValue(0) <= 0)
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto page = MedicationCatalogPageVO::createShared();
	page->pageNo = query->pageNo;
	page->pageSize = query->pageSize;
	result->success(page);
	return result;
}

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execQueryMedicationCatalog(const String& id)
{
	auto result = MedicationCatalogDetailJsonVO::createShared();
	if (!hasText(id))
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto detail = MedicationCatalogDetailVO::createShared();
	detail->id = id;
	detail->medicationDefId = id;
	result->success(detail);
	return result;
}

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execAddMedicationCatalog(
	const MedicationCatalogAddDTO::Wrapper& dto)
{
	auto result = MedicationCatalogDetailJsonVO::createShared();
	if (!hasRequiredMedicationFields(dto))
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto detail = MedicationCatalogDetailVO::createShared();
	applyMedicationFields(dto, detail);
	result->success(detail);
	return result;
}

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execModifyMedicationCatalog(
	const MedicationCatalogModifyDTO::Wrapper& dto)
{
	auto result = MedicationCatalogDetailJsonVO::createShared();
	if (!hasRequiredMedicationFields(dto) || !hasText(dto->id))
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto detail = MedicationCatalogDetailVO::createShared();
	detail->id = dto->id;
	detail->medicationDefId = dto->id;
	applyMedicationFields(dto, detail);
	result->success(detail);
	return result;
}

NoDataJsonVO::Wrapper MedicationCatalogController::execRemoveMedicationCatalog(const List<String>& ids)
{
	return createBatchResult(ids);
}

NoDataJsonVO::Wrapper MedicationCatalogController::execEnableMedicationCatalog(const List<String>& ids)
{
	return createBatchResult(ids);
}

NoDataJsonVO::Wrapper MedicationCatalogController::execDisableMedicationCatalog(const List<String>& ids)
{
	return createBatchResult(ids);
}
