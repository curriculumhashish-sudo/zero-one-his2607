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
		return hasText(value)
			&& value->find_first_not_of("0123456789") == std::string::npos;
	}

	bool hasRequiredEditFields(const MedicationCatalogModifyDTO::Wrapper& dto)
	{
		return dto
			&& isUnsignedIntegerId(dto->id)
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

MedicationCatalogDetailJsonVO::Wrapper MedicationCatalogController::execEditMedicationCatalog(
	const MedicationCatalogModifyDTO::Wrapper& dto)
{
	auto result = MedicationCatalogDetailJsonVO::createShared();
	if (!hasRequiredEditFields(dto))
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}

	try
	{
		const auto& mapper = getDefaultObjectMapper();
		if (!mapper)
		{
			result->init(nullptr, RS_PARAMS_INVALID);
			return result;
		}

		auto detail = mapper->readFromString<MedicationCatalogDetailVO::Wrapper>(mapper->writeToString(dto));
		detail->specification = dto->totalVolume;
		if (!hasText(detail->medicationDefId))
		{
			detail->medicationDefId = dto->id;
		}
		result->success(detail);
	}
	catch (const std::exception&)
	{
		result->init(nullptr, RS_PARAMS_INVALID);
	}
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
