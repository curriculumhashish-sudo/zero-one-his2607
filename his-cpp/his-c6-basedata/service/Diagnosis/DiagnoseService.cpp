/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei

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
#include "DiagnoseService.h"
#include "../../dao/Diagnosis/DiagnosisDAO.h"
#include "../../lib-common/include/id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include <cerrno>
#include <climits>
#include <cstdlib>

namespace
{
	bool tryParseInt32(const std::string& value, int& parsedValue)
	{
		if (value.empty())
		{
			return false;
		}
		char* end = nullptr;
		errno = 0;
		const long valueAsLong = std::strtol(value.c_str(), &end, 10);
		if (errno != 0 || end == value.c_str() || *end != '\0'
			|| valueAsLong < INT_MIN || valueAsLong > INT_MAX)
		{
			return false;
		}
		parsedValue = static_cast<int>(valueAsLong);
		return true;
	}

	bool isTraditionalChineseMedicineSource(const std::string& sourceEnum)
	{
		return sourceEnum == "tcm_dx" || sourceEnum == "tcm_syndrome";
	}

	bool isWesternMedicineSource(const std::string& sourceEnum)
	{
		return sourceEnum == "general_dx"
			|| sourceEnum == "chronic_spec"
			|| sourceEnum == "disease_pkg"
			|| sourceEnum == "paid_dx"
			|| sourceEnum == "day_surgery_dx"
			|| sourceEnum == "tumor_morph";
	}

	std::string getSourceEnumText(const std::string& sourceEnum)
	{
		if (sourceEnum == "general_dx")
			return ZH_WORDS_GETTER("diagnose.source.general_dx");
		if (sourceEnum == "chronic_spec")
			return ZH_WORDS_GETTER("diagnose.source.chronic_spec");
		if (sourceEnum == "disease_pkg")
			return ZH_WORDS_GETTER("diagnose.source.disease_pkg");
		if (sourceEnum == "paid_dx")
			return ZH_WORDS_GETTER("diagnose.source.paid_dx");
		if (sourceEnum == "day_surgery_dx")
			return ZH_WORDS_GETTER("diagnose.source.day_surgery_dx");
		if (sourceEnum == "tcm_dx")
			return ZH_WORDS_GETTER("diagnose.source.tcm_dx");
		if (sourceEnum == "tcm_syndrome")
			return ZH_WORDS_GETTER("diagnose.source.tcm_syndrome");
		if (sourceEnum == "tumor_morph")
			return ZH_WORDS_GETTER("diagnose.source.tumor_morph");
		return "";
	}
}

DiagnosisSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
	const DiagnosisSelectQuery::Wrapper& query)
{
	auto page = DiagnosisSelectPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	DiagnosisDAO dao;
	const uint64_t total = dao.count(query);
	page->total = static_cast<int64_t>(total);
	page->calcPages();
	if (total == 0)
	{
		return page;
	}

	std::list<ConditionDefinitionDO> records = dao.selectWithPage(query);
	for (ConditionDefinitionDO& record : records)
	{
		auto dto = DiagnosisSelectDTO::createShared();
		dto->conditionDefinitionId = record.getId();
		dto->conditionCode = record.getConditionCode();
		dto->conditionDefinitionName = record.getName();
		dto->pyStr = record.getPyStr();
		dto->wbStr = record.getWbStr();

		const std::string sourceEnum = record.getSourceEnum();
		const bool isTcm = isTraditionalChineseMedicineSource(sourceEnum);
		if (isTcm)
		{
			dto->diagnosisKind = "TCM";
			dto->diagnosisKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
		}
		else if (isWesternMedicineSource(sourceEnum))
		{
			dto->diagnosisKind = "WESTERN";
			dto->diagnosisKindText = ZH_WORDS_GETTER("diagnose.type.western");
		}
		else
		{
			dto->diagnosisKind = nullptr;
			dto->diagnosisKindText = nullptr;
		}

		int sourceEnumValue = 0;
		if (tryParseInt32(sourceEnum, sourceEnumValue))
		{
			dto->sourceEnum = sourceEnumValue;
		}
		else
		{
			dto->sourceEnum = nullptr;
		}
		const std::string sourceEnumText = getSourceEnumText(sourceEnum);
		dto->sourceEnumText = sourceEnumText.empty()
			? oatpp::String(nullptr)
			: oatpp::String(sourceEnumText.c_str());

		const int ybFlag = record.getYbFlag() == 1 ? 1 : 0;
		dto->ybFlag = ybFlag;
		dto->ybFlagText = ybFlag == 1
			? ZH_WORDS_GETTER("diagnose.flag.yes")
			: ZH_WORDS_GETTER("diagnose.flag.no");
		dto->statusEnum = 2;
		page->addData(dto);
	}
	return page;
}

std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
{
	DiagnosisDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		DiagnosisName, name,
		DiagnosisType, diagType,
		DiagnosisCode, chronicCode,
		MedicalInsuranceCode, medicalInsuranceCode,
		Status, status,
		IsDelete, isDelete
	);

	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		MedicalInsuranceTag, medicalInsuranceTag,
		MedicalInsuranceMatch, medicalInsuranceMatch,
		PinyinCode, pinyinCode,
		WubiCode, wubiCode,
		Remark, remark,
		CatName, catName,
		TenantId,tenantId,
		Source_enum, source_enum);

	UuidFacade uuidFacade;
	data.setId(uuidFacade.genUuid());
	data.setCreateBy(dto->getPayload()->getUsername());
	data.setCreateTime(SimpleDateTimeFormat::format());
	//data.setIsDelete(0);
	data.setVersion(1);

	DiagnosisDAO dao;
	return dao.insert(data) == 1 ? data.getId() : "";
}

bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
{
	DiagnosisDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		DiagnosisName, name,
		DiagnosisType, diagType,
		DiagnosisCode, chronicCode,
		MedicalInsuranceCode, medicalInsuranceCode,
		Status, status,
		TenantId, tenantId,
		IsDelete, isDelete,
		Id, id);

	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		MedicalInsuranceTag, medicalInsuranceTag,
		MedicalInsuranceMatch, medicalInsuranceMatch,
		PinyinCode, pinyinCode,
		WubiCode, wubiCode,
		Remark, remark,
		Source_enum,source_enum,
	Version,version);

	data.setUpdateBy(dto->getPayload()->getUsername());
	data.setUpdateTime(SimpleDateTimeFormat::format());

	DiagnosisDAO dao;
	return dao.update(data) == 1;
}

//删除一条数据（前端有）
bool DiagnoseService::removeOne(const std::string& id)
{
	DiagnosisDO data;
	data.setIsDelete(1);
	data.setUpdateTime(SimpleDateTimeFormat::format());

	DiagnosisDAO dao;
	return dao.update(data) == 1;
}
//物理批量删除
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
	std::list<std::string> dataIds;
	for (const auto& id : *ids.get())
	{
		dataIds.emplace_back(id.getValue({}));
	}

	DiagnosisDAO dao;
	return dao.deleteByIds<DiagnosisDO>(dataIds) == ids->size();
}

bool DiagnoseService::batchChangeStatus(
	const oatpp::List<oatpp::String>& ids,
	oatpp::String status)
{
	DiagnosisDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnosisDO data;
		data.setId(idItem.getValue({}));
		data.setStatus(*status);
		data.setUpdateTime(SimpleDateTimeFormat::format());
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;
}
