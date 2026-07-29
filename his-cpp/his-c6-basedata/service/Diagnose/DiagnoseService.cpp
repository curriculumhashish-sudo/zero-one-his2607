//<<<<<<< HEAD
// #include "DiagnoseService.h"
//#include "../../dao/Diagnose/DiagnoseDAO.h"
//#include "../../lib-common/include/id/UuidFacade.h"
//=======
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
#include "../../dao/Diagnose/DiagnoseDAO.h"
#include "../../domain/DiagnoseConstants.h"

#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "StringUtil.h"

#include <algorithm>
#include <charconv>

//#include <charconv>
//static const std::string SOURCE_GENERAL_diagnose = "general_dx";
//static const std::string SOURCE_CHRONIC_SPECIAL = "chronic_spec";
//static const std::string SOURCE_DISEASE_PACKAGE = "disease_pkg";
//static const std::string SOURCE_PAID_diagnose = "paid_dx";
//static const std::string SOURCE_DAY_SURGERY = "day_surgery_dx";
//static const std::string SOURCE_TCM_diagnose = "tcm_dx";
//static const std::string SOURCE_TCM_SYNDROME = "tcm_syndrome";
//static const std::string SOURCE_TUMOR_MORPHOLOGY = "tumor_morph";
//static const std::string diagnose_KIND_TCM = "TCM";
//static const std::string diagnose_KIND_WESTERN = "WESTERN";
//static const std::string STATUS_ACTIVE = "ACTIVE";
//static constexpr int STATUS_ACTIVE_SORT_ORDER = 2;
//
//static bool tryParseInt32(const std::string& value, int32_t& result)
//{
//	if (value.empty())
//	{
//		return false;
//	}
//	const char* begin = value.data();
//	const char* end = begin + value.size();
//	const auto parsed = std::from_chars(begin, end, result);
//	return parsed.ec == std::errc() && parsed.ptr == end;
//}
//
//static bool isTraditionalChineseMedicineSource(const std::string& sourceEnum)
//{
//	return sourceEnum == SOURCE_TCM_diagnose || sourceEnum == SOURCE_TCM_SYNDROME;
//}
//
//static bool isWesternMedicineSource(const std::string& sourceEnum)
//{
//	return sourceEnum == SOURCE_GENERAL_diagnose
//		|| sourceEnum == SOURCE_CHRONIC_SPECIAL
//		|| sourceEnum == SOURCE_DISEASE_PACKAGE
//		|| sourceEnum == SOURCE_PAID_diagnose
//		|| sourceEnum == SOURCE_DAY_SURGERY
//		|| sourceEnum == SOURCE_TUMOR_MORPHOLOGY;
//}
//
//static std::string getSourceEnumText(const std::string& sourceEnum)
//{
//	if (!isWesternMedicineSource(sourceEnum) &&
//		!isTraditionalChineseMedicineSource(sourceEnum))
//	{
//		return "";
//	}
//	return ZH_WORDS_GETTER("diagnose.source." + sourceEnum);
//}
//
//DiagnoseSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
//	const DiagnoseSelectQuery::Wrapper& query)
//{
//	auto page = DiagnoseSelectPageDTO::createShared();
//	page->pageIndex = query->pageIndex;
//	page->pageSize = query->pageSize;
//
//	DiagnoseDAO dao;
//	const uint64_t total = dao.count(query);
////DiagnoseSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
////	const DiagnoseSelectQuery::Wrapper& query)
////{
////	auto page = DiagnoseSelectPageDTO::createShared();
////	page->pageIndex = query->pageIndex;
////	page->pageSize = query->pageSize;
////
////	DiagnoseDAO dao;
////	const uint64_t total = dao.count(query);
//	//DiagnoseSelectPageDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
//	//	const DiagnoseSelectQuery::Wrapper & query,
//	//	const PayloadDTO & payload)
//	//{
//	//	if (!query || !query->validate().empty() || payload.getId().empty())
//	//	{
//	//		return nullptr;
//	//	}
//
//	//	DiagnoseDAO dao;
//	//	const std::string tenantId = dao.selectCurrentTenantId(payload.getId());
//	//	if (tenantId.empty())
//	//	{
//	//		return nullptr;
//	//	}
//
//	//	auto page = DiagnoseSelectPageDTO::createShared();
//	//	page->pageIndex = query->pageIndex;
//	//	page->pageSize = query->pageSize;
//
//	//	const uint64_t total = dao.count(query, tenantId);
//	//	page->total = static_cast<int64_t>(total);
//	//	page->calcPages();
//	//	if (total == 0)
//	//	{
//	//		return page;
//	//	}
//
//		std::list<ClnConditionDefinitionDO> records = dao.selectWithPage(query, tenantId);
//		for (ClnConditionDefinitionDO& record : records)
//		{
//			auto dto = DiagnoseSelectDTO::createShared();
//			dto->conditionDefinitionId = record.getId();
//			dto->conditionCode = record.getConditionCode();
//			dto->conditionDefinitionName = record.getName();
//			dto->pyStr = record.getPyStr();
//			dto->wbStr = record.getWbStr();
//			auto dto = DiagnoseSelectDTO::createShared();
//			ZO_STAR_DOMAIN_DO_TO_DTO(
//				dto,
//				record,
//				conditionDefinitionId, Id,
//				conditionCode, ConditionCode,
//				conditionDefinitionName, Name,
//				pyStr, PyStr,
//				wbStr, WbStr);
//
//			const std::string sourceEnum = record.getSourceEnum();
//			const bool isTcm = isTraditionalChineseMedicineSource(sourceEnum);
//			if (isTcm)
//			{
//				dto->diagnoseKind = "TCM";
//				dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
//			}
//			else if (isWesternMedicineSource(sourceEnum))
//			{
//				dto->diagnoseKind = "WESTERN";
//				dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.western");
//	//=======
//				dto->diagnoseKind = diagnose_KIND_TCM;
//				dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
//			}
//			else if (isWesternMedicineSource(sourceEnum))
//			{
//				dto->diagnoseKind = DIAGNOSIS_KIND_WESTERN;
//				dto->diagnoseKindText = ZH_WORDS_GETTER("diagnose.type.western");
//			}
//			else
//			{
//				dto->diagnoseKind = nullptr;
//				dto->diagnoseKindText = nullptr;
//			}
//
//			int32_t sourceEnumValue = 0;
//			dto->sourceEnum = tryParseInt32(sourceEnum, sourceEnumValue)
//				? oatpp::Int32(sourceEnumValue)
//				: oatpp::Int32(nullptr);
//			const std::string sourceEnumText = getSourceEnumText(sourceEnum);
//			dto->sourceEnumText = sourceEnumText.empty()
//				? oatpp::String(nullptr)
//				: oatpp::String(sourceEnumText.c_str());
//
//			const int ybFlag = record.getYbFlag() == 1 ? 1 : 0;
//			dto->ybFlag = ybFlag;
//			dto->ybFlagText = ybFlag == 1
//				? ZH_WORDS_GETTER("diagnose.flag.yes")
//				: ZH_WORDS_GETTER("diagnose.flag.no");
//			dto->statusEnum = record.getStatusEnum() == STATUS_ACTIVE
//				? oatpp::Int32(STATUS_ACTIVE_SORT_ORDER)
//				: oatpp::Int32(nullptr);
//			page->addData(dto);
//		}
//		
//		
//		return page;
//
//
//	}
//  
DiagnoseSelectDTO::Wrapper DiagnoseService::listSelectableDiagnoses(
	const DiagnoseSelectQuery::Wrapper& query,
	const PayloadDTO& payload)
{
	// 校验查询参数和 JWT 用户标识。
	if (!query || !query->validate().empty() || payload.getId().empty())
	{
		return nullptr;
	}

	DiagnoseDAO dao;
	// 解析当前用户所属的有效租户。
	const std::string tenantId = dao.selectCurrentTenantId(payload.getId());
	if (tenantId.empty())
	{
		return nullptr;
	}

	// 初始化分页结果并统计总数。
	auto page = DiagnoseSelectDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	const uint64_t total = dao.countSelectableDiagnoses(query, tenantId);
	page->total = static_cast<int64_t>(total);
	page->calcPages();
	if (total == 0)
	{
		return page;
	}

	// 分页读取诊疗定义 DO。
	std::list<ClnConditionDefinitionDO> records =
		dao.selectSelectableDiagnosesWithPage(query, tenantId);
	for (ClnConditionDefinitionDO& record : records)
	{
		auto dto = DiagnoseSelectDTO::createShared();
		// 使用公共宏转换同义字段。
		ZO_STAR_DOMAIN_DO_TO_DTO(
			dto,
			record,
			conditionDefinitionId, Id,
			conditionCode, ConditionCode,
			conditionDefinitionName, Name,
			pyStr, PyStr,
			wbStr, WbStr);

		std::string sourceEnum = record.getSourceEnum();
		StringUtil::trim(&sourceEnum);
		dto->sourceEnum = nullptr;
		int sourceValue = 0;
		// 参考项目对数字来源枚举进行安全解析。
		const auto parseResult = std::from_chars(
			sourceEnum.data(),
			sourceEnum.data() + sourceEnum.size(),
			sourceValue);
		if (parseResult.ec == std::errc() &&
			parseResult.ptr == sourceEnum.data() + sourceEnum.size())
		{
			dto->sourceEnum = sourceValue;
		}

		// 根据来源编码区分中医和西医。
		const bool isTraditionalChineseMedicineSource =
			std::find(
				DiagnoseConstants::TCM_SOURCE_CODES.begin(),
				DiagnoseConstants::TCM_SOURCE_CODES.end(),
				sourceEnum) != DiagnoseConstants::TCM_SOURCE_CODES.end();
		const bool isWesternMedicineSource =
			std::find(
				DiagnoseConstants::WESTERN_SOURCE_CODES.begin(),
				DiagnoseConstants::WESTERN_SOURCE_CODES.end(),
				sourceEnum) != DiagnoseConstants::WESTERN_SOURCE_CODES.end();

		if (isTraditionalChineseMedicineSource)
		{
			dto->diagnosisKind = DiagnoseConstants::KIND_TCM;
			dto->diagnosisKindText = ZH_WORDS_GETTER("diagnose.type.tcm");
		}
		else if (isWesternMedicineSource)
		{
			dto->diagnosisKind = DiagnoseConstants::KIND_WESTERN;
			dto->diagnosisKindText = ZH_WORDS_GETTER("diagnose.type.western");
		}
		else
		{
			dto->diagnosisKind = nullptr;
			dto->diagnosisKindText = nullptr;
		}

		// 从 UTF-8 中文词典读取来源名称。
		const std::string sourceEnumText =
			isTraditionalChineseMedicineSource || isWesternMedicineSource
			? ZH_WORDS_GETTER("diagnose.source." + sourceEnum)
			: "";
		dto->sourceEnumText = sourceEnumText.empty()
			? oatpp::String(nullptr)
			: oatpp::String(sourceEnumText.c_str());

		// 转换医保标记和启用状态。
		const int ybFlag = record.getYbFlag() == 1 ? 1 : 0;
		dto->ybFlag = ybFlag;
		dto->ybFlagText = ybFlag == 1
			? ZH_WORDS_GETTER("diagnose.flag.yes")
			: ZH_WORDS_GETTER("diagnose.flag.no");
		dto->statusEnum =
			record.getStatusEnum() == DiagnoseConstants::STATUS_ACTIVE
			? oatpp::Int32(DiagnoseConstants::STATUS_ACTIVE_SORT_ORDER)
			: oatpp::Int32(nullptr);
		page->addData(dto);
	}
	return page;
}


//std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
//{
//	DiagnosisDO data;
//	ZO_STAR_DOMAIN_DTO_TO_DO(
//		data,
//		dto,
//		DiagnosisName, name,
//		DiagnosisType, diagType,
//		DiagnosisCode, chronicCode,
//		MedicalInsuranceCode, medicalInsuranceCode,
//		Status, status,
//		IsDelete, isDelete
//	);
//
//	ZO_STAR_DOMAIN_DTO_TO_DO(
//		data,
//		dto,
//		MedicalInsuranceTag, medicalInsuranceTag,
//		MedicalInsuranceMatch, medicalInsuranceMatch,
//		PinyinCode, pinyinCode,
//		WubiCode, wubiCode,
//		Remark, remark,
//		CatName, catName,
//		TenantId,tenantId,
//		Source_enum, source_enum);
//
//	UuidFacade uuidFacade;
//	data.setId(uuidFacade.genUuid());
//	data.setCreateBy(dto->getPayload()->getUsername());
//	data.setCreateTime(SimpleDateTimeFormat::format());
//	//data.setIsDelete(0);
//	data.setVersion(1);
//
//	DiagnosisDAO dao;
//	return dao.insert(data) == 1 ? data.getId() : "";
//}
//
//bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
//{
//	DiagnosisDO data;
//	ZO_STAR_DOMAIN_DTO_TO_DO(
//		data,
//		dto,
//		DiagnosisName, name,
//		DiagnosisType, diagType,
//		DiagnosisCode, chronicCode,
//		MedicalInsuranceCode, medicalInsuranceCode,
//		Status, status,
//		TenantId, tenantId,
//		IsDelete, isDelete,
//		Id, id);
//
//	ZO_STAR_DOMAIN_DTO_TO_DO(
//		data,
//		dto,
//		MedicalInsuranceTag, medicalInsuranceTag,
//		MedicalInsuranceMatch, medicalInsuranceMatch,
//		PinyinCode, pinyinCode,
//		WubiCode, wubiCode,
//		Remark, remark,
//		Source_enum,source_enum,
//	Version,version);
//
//	data.setUpdateBy(dto->getPayload()->getUsername());
//	data.setUpdateTime(SimpleDateTimeFormat::format());
//	data.setVersion(*(dto->version)+1);
//	DiagnosisDAO dao;
//	return dao.update(data) == 1;
//}
//
////删除一条数据（前端有）
//bool DiagnoseService::removeOne(const std::string& id)
//{
//	DiagnosisDO data;
//	data.setIsDelete(1);
//	data.setUpdateTime(SimpleDateTimeFormat::format());
//
//	DiagnosisDAO dao;
//	return dao.update(data) == 1;
//}
////物理批量删除
//bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
//{
//	std::list<std::string> dataIds;
//	for (const auto& id : *ids.get())
//	{
//		dataIds.emplace_back(id.getValue({}));
//	}
//
//	DiagnosisDAO dao;
//	return dao.deleteByIds<DiagnosisDO>(dataIds) == ids->size();
//}
//
//bool DiagnoseService::batchChangeStatus(const DiagnoseBatchStatusDTO::Wrapper& dto,oatpp::String status)
//{
//	const oatpp::List<DiagnoseBatchDTO::Wrapper> ids = dto->ids;
//	DiagnosisDAO dao;
//	int successCount = 0;
//	for (const auto& idItem : *ids.get())
//	{
//		DiagnosisDO data;
//		data.setId(idItem->id);
//		data.setStatus(*status);
//		data.setUpdateBy(dto->getPayload()->getUsername());
//		data.setUpdateTime(SimpleDateTimeFormat::format());
//		data.setVersion((idItem->version)+ 1);
//		if (dao.update(data) == 1)
//		{
//			++successCount;
//		}
//	}
//	return successCount > 0;
//}
////逻辑批量删除
//bool DiagnoseService::execRemoveData(const DiagnoseRemoveDTO::Wrapper& dto)
//{
//	const oatpp::List<oatpp::String> ids = dto->ids;
//	DiagnosisDAO dao;
//	int successCount = 0;
//	for (const auto& idItem : *ids.get())//返回原生的vector并枚举遍历，get获取裸指针
//	{
//		DiagnosisDO data;
//		data.setId(idItem.getValue({}));
//		data.setDeletedBy(dto->getPayload()->getUsername());
//		data.setDeletedTime(SimpleDateTimeFormat::format());
//		data.setIsDelete(1);
//		if (dao.update(data) == 1)
//		{
//			++successCount;
//		}
//	}
//	return successCount > 0;
//
//}

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
	data.setVersion(*(dto->version)+1);
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
bool DiagnoseService::RemoveData(const oatpp::List<oatpp::String>& ids)
{
	std::list<std::string> dataIds;
	for (const auto& id : *ids.get())
	{
		dataIds.emplace_back(id.getValue({}));
	}

	DiagnosisDAO dao;
	return dao.deleteByIds<DiagnosisDO>(dataIds) == ids->size();
}

//逻辑批量删除(不记录删除人)
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
	DiagnosisDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())//返回原生的vector并枚举遍历，get获取裸指针
	{
		DiagnosisDO data;
		data.setId(idItem.getValue({}));
		data.setDeletedTime(SimpleDateTimeFormat::format());
		data.setIsDelete(1);
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;
	
}

bool DiagnoseService::batchChangeStatus(const DiagnoseBatchStatusDTO::Wrapper& dto,oatpp::String status)
{
	const oatpp::List<DiagnoseBatchDTO::Wrapper> ids = dto->ids;
	DiagnosisDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		DiagnosisDO data;
		data.setId(idItem->id);
		data.setStatus(*status);
		data.setUpdateBy(dto->getPayload()->getUsername());
		data.setUpdateTime(SimpleDateTimeFormat::format());
		data.setVersion((idItem->version)+ 1);
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;
}
//逻辑批量删除
bool DiagnoseService::execRemoveData(const DiagnoseRemoveDTO::Wrapper& dto)
{
	const oatpp::List<oatpp::String> ids = dto->ids;
	DiagnosisDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())//返回原生的vector并枚举遍历，get获取裸指针
	{
		DiagnosisDO data;
		data.setId(idItem.getValue({}));
		data.setDeletedBy(dto->getPayload()->getUsername());
		data.setDeletedTime(SimpleDateTimeFormat::format());
		data.setIsDelete(1);
		if (dao.update(data) == 1)
		{
			++successCount;
		}
	}
	return successCount > 0;

}
