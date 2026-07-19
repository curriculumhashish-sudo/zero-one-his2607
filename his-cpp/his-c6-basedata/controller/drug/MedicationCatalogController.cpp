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
	template<class T>
	void applyEditableFields(const T& dto, const MedicationCatalogDetailDTO::Wrapper& detail)
	{
		if (!dto)
		{
			return;
		}
		detail->tenantId = dto->tenantId;
		detail->orgId = dto->orgId;
		detail->busNo = dto->busNo;
		detail->name = dto->name;
		detail->categoryCode = dto->categoryCode;
		detail->merchandiseName = dto->merchandiseName;
		detail->purchaseUnitCode = dto->purchaseUnitCode;
		detail->splitUnitCode = dto->splitUnitCode;
		detail->doseUnitCode = dto->doseUnitCode;
		detail->packageUnitCode = dto->packageUnitCode;
		detail->stockUnitCode = dto->stockUnitCode;
		detail->manufacturerText = dto->manufacturerText;
		detail->supplyId = dto->supplyId;
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
		result->init(RS_SUCCESS);
		return result;
	}
}

MedicationCatalogPageJsonVO::Wrapper MedicationCatalogController::execPageMedicationCatalog(
	const MedicationCatalogPageQuery::Wrapper& query)
{
	auto page = MedicationCatalogPageDTO::createShared();
	if (query)
	{
		page->pageNo = query->pageNo;
		page->pageSize = query->pageSize;
	}
	auto result = MedicationCatalogPageJsonVO::createShared();
	result->success(page);
	return result;
}

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execGetMedicationCatalog(const String& id)
{
	auto detail = MedicationCatalogDetailDTO::createShared();
	if (id && !id->empty())
	{
		detail->id = id;
		detail->medicationDefId = id;
	}
	auto result = MedicationCatalogDetailJsonVO::createShared();
	result->success(detail);
	return result;
}

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execCreateMedicationCatalog(
	const MedicationCatalogCreateDTO::Wrapper& dto)
{
	auto result = MedicationCatalogDetailJsonVO::createShared();
	if (!dto)
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto detail = MedicationCatalogDetailDTO::createShared();
	applyEditableFields(dto, detail);
	result->success(detail);
	return result;
}

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execEditMedicationCatalog(
	const MedicationCatalogUpdateDTO::Wrapper& dto)
{
	auto result = MedicationCatalogDetailJsonVO::createShared();
	if (!dto || !dto->id || dto->id->empty())
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}
	auto detail = MedicationCatalogDetailDTO::createShared();
	detail->id = dto->id;
	detail->medicationDefId = dto->id;
	applyEditableFields(dto, detail);
	result->success(detail);
	return result;
}

NoDataJsonVO::Wrapper MedicationCatalogController::execDeleteMedicationCatalog(const List<String>& ids)
{
	return createBatchResult(ids);
}

NoDataJsonVO::Wrapper MedicationCatalogController::execStartMedicationCatalog(const List<String>& ids)
{
	return createBatchResult(ids);
}

NoDataJsonVO::Wrapper MedicationCatalogController::execStopMedicationCatalog(const List<String>& ids)
{
	return createBatchResult(ids);
}
