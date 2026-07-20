/*
 Copyright Zero One Star. All rights reserved.

 @Author: bandao
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

	bool isUnsignedIntegerId(const oatpp::String& value)
	{
		if (!hasText(value))
		{
			return false;
		}
		return value->find_first_not_of("0123456789") == std::string::npos;
	}

	template<class T>
	bool hasRequiredMedicationFields(const T& dto)
	{
		return dto
			&& hasText(dto->name)
			&& hasText(dto->merchandiseName)
			&& hasText(dto->categoryCode)
			&& hasText(dto->manufacturerText)
			&& hasText(dto->supplyId)
			&& hasText(dto->typeCode)
			&& hasText(dto->pharmacologyCategoryCode)
			&& hasText(dto->totalVolume)
			&& hasText(dto->purchaseUnitCode)
			&& hasText(dto->splitUnitCode)
			&& hasText(dto->doseUnitCode)
			&& hasText(dto->doseFormCode)
			&& hasText(dto->packageUnitCode)
			&& hasText(dto->stockUnitCode)
			&& hasText(dto->partAttributeEnum)
			&& hasText(dto->thoPartAttributeEnum)
			&& dto->doseUnitRatio.getValue(0.0) > 0.0
			&& dto->purchaseUnitRatio.getValue(0.0) > 0.0
			&& dto->purchasePrice != nullptr
			&& dto->stockUnitRatio.getValue(0.0) > 0.0
			&& dto->retailPrice != nullptr
			&& dto->packageUnitRatio.getValue(0.0) > 0.0;
	}

	template<class T>
	void applyMedicationFields(const T& dto, const MedicationCatalogDetailVO::Wrapper& detail)
	{
		if (!dto)
		{
			return;
		}
		detail->orgId = dto->orgId;
		detail->medicationDefId = dto->medicationDefId;
		detail->busNo = dto->busNo;
		detail->name = dto->name;
		detail->domainEnum = dto->domainEnum;
		detail->version = dto->version;
		detail->nameEn = dto->nameEn;
		detail->pyStr = dto->pyStr;
		detail->wbStr = dto->wbStr;
		detail->categoryCode = dto->categoryCode;
		detail->merchandiseName = dto->merchandiseName;
		detail->merchandisePyStr = dto->merchandisePyStr;
		detail->merchandiseWbStr = dto->merchandiseWbStr;
		detail->purchaseUnitCode = dto->purchaseUnitCode;
		detail->purchaseUnitCode_dictText = dto->purchaseUnitCode_dictText;
		detail->splitUnitCode = dto->splitUnitCode;
		detail->comprisedText = dto->comprisedText;
		detail->ingredient = dto->ingredient;
		detail->doseFrom = dto->doseFrom;
		detail->doseFrom_dictText = dto->doseFrom_dictText;
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
		detail->nationalDrugCode = dto->nationalDrugCode;
		detail->partAttributeEnum = dto->partAttributeEnum;
		detail->antibioticCode = dto->antibioticCode;
		detail->antibioticCode_dictText = dto->antibioticCode_dictText;
		detail->restrictedEnum = dto->restrictedEnum;
		detail->selfFlag = dto->selfFlag;
		detail->antibioticFlag = dto->antibioticFlag;
		detail->basicFlag = dto->basicFlag;
		detail->thoPartAttributeEnum = dto->thoPartAttributeEnum;
		detail->chrgitmLv = dto->chrgitmLv;
		detail->typeCode = dto->typeCode;
		detail->typeCode_dictText = dto->typeCode_dictText;
		detail->ybType = dto->ybType;
		detail->rxFlag = dto->rxFlag;
		detail->dosageInstruction = dto->dosageInstruction;
		detail->drug69Code = dto->drug69Code;
		detail->totalVolume = dto->totalVolume;
		detail->doseFormCode = dto->doseFormCode;
		detail->doseUnitCode_dictText = dto->doseUnitCode_dictText;
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
		detail->statusEnum_enumText = dto->statusEnum_enumText;
		detail->activeFlag = dto->activeFlag;
		detail->purchasePrice = dto->purchasePrice;
		detail->retailPrice = dto->retailPrice;
		detail->maximumRetailPrice = dto->maximumRetailPrice;
		detail->specification = dto->totalVolume;
		if (dto->statusEnum && dto->statusEnum == "INACTIVE")
		{
			detail->statusEnum_enumText = u8"\u5DF2\u505C\u7528";
		}
		if (dto->activeFlag != nullptr && !dto->activeFlag.getValue(false))
		{
			detail->activeFlagText = u8"\u505C\u7528";
		}
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
			if (!isUnsignedIntegerId(id))
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
	page->totalElements = page->totalCount;
	const auto pageSize = query->pageSize.getValue(1);
	const auto totalCount = page->totalCount.getValue(0);
	page->totalPages = static_cast<v_int32>((totalCount + pageSize - 1) / pageSize);
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

MedicationCatalogInitJsonVO::Wrapper MedicationCatalogController::execGetMedicationInit(
	const MedicationCatalogInitDTO::Wrapper& dto)
{
	auto result = MedicationCatalogInitJsonVO::createShared();
	if (!dto)
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	result->success(MedicationCatalogInitVO::createShared());
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
	if (!hasRequiredMedicationFields(dto) || !isUnsignedIntegerId(dto->id))
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto detail = MedicationCatalogDetailVO::createShared();
	applyMedicationFields(dto, detail);
	detail->id = dto->id;
	if (!hasText(detail->medicationDefId))
	{
		detail->medicationDefId = dto->id;
	}
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
