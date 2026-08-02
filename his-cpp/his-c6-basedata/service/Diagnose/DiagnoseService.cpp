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
//这两个别注释
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

//#include "StringUtil.h"
//
//#include <algorithm>
//#include <charconv>

// 1. 弹窗选择分页
DiagnoseSelectDTO::Wrapper DiagnoseService::listDiagnoseSelect(const DiagnoseSelectQuery::Wrapper& query)
{
	// 定义分页对象
	auto page = DiagnoseSelectDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	//// 获取数据总条数
	DiagnoseDAO dao;
	auto count = dao.count(query);//这里跟service调用传入的参数跟定义的count函数参数类型对不上，这里按照在DAO重载了一个count函数
	if (count <= 0)
		return page;
	page->total = count;
	page->calcPages();

	// 获取分页数据
	auto list = dao.selectAll(query);
	for (auto one : list)
	{
		//auto dto = DiagnoseSelectDTO::createShared();        这里不是应该是选择的那个类，而是放到选择里里的那个类
		auto dto = DSelectDTO::createShared();

		ZO_STAR_DOMAIN_DO_TO_DTO_1(
			dto, one,
			id, Id,
			diagnoseCode, ConditionCode,
			diagnoseName, Name,
			dictType, TypeCode
		);
		page->addData(dto);
	}
	return page;
}

DiagnosePageDTO::Wrapper DiagnoseService::listDiagnosePage(const DiagnosePageQuery::Wrapper& query)
{
	// 定义分页对象
	auto page = DiagnosePageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	// 获取数据总条数
	DiagnoseDAO dao;
	auto count = dao.count(query);
	if (count <= 0)
		return page;
	page->total = count;
	page->calcPages();

	// 获取分页数据
	auto list = dao.selectAll(query);
	
	for (auto one : list)
	{
		//auto dto = DiagnosePageDTO::createShared();             修正
		auto dto = DPageDTO::createShared();//给自己做笔记:这里的类不能继承PageDTO
		
		ZO_STAR_DOMAIN_DO_TO_DTO_1(
			dto, one,
			id, Id,
			diagnoseCode, ConditionCode,
			diagnoseName, Name,
			diagnoseType, TypeCode,
			medicalInsuranceCode, YbNo,
			status, StatusEnum,
			remark, Description
			
		);
		page->addData(dto);
	}
	return page;
}

// 根据ID查询详情 
DiagnoseDetailDTO::Wrapper DiagnoseService::getDiagnoseDetail(const oatpp::String& id)
{
	// 获取单条主数据
	DiagnoseDAO dao;
	auto data = dao.selectById(id.getValue(""));

	// 如果数据不存在，直接返回空
	if (!data)
		return nullptr;

	// 创建 DTO 对象并进行字段映射
	//auto dto = DiagnoseDetailDTO::createShared();            修正
	auto dto = DiagnoseDetailDTO::createShared();
	ZO_STAR_DOMAIN_DO_TO_DTO_1(
		dto, data,
		id, Id,
		diagnoseCode, ConditionCode,
		diagnoseName, Name,
		diagnoseType, TypeCode,
		medicalInsuranceCode, YbNo,
		status, StatusEnum,
		remark, Description
	);

	 //返回详情数据
	return dto;
	
	
}

//std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
//{
//	diagnoseDO data;
//	ZO_STAR_DOMAIN_DTO_TO_DO(
//		data,
//		dto,
//		diagnoseName, name,
//		diagnoseType, diagType,
//		diagnoseCode, chronicCode,
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
//	diagnoseDAO dao;
//	return dao.insert(data) == 1 ? data.getId() : "";
//}
//
//bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
//{
//	diagnoseDO data;
//	ZO_STAR_DOMAIN_DTO_TO_DO(
//		data,
//		dto,
//		diagnoseName, name,
//		diagnoseType, diagType,
//		diagnoseCode, chronicCode,
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
//	diagnoseDAO dao;
//	return dao.update(data) == 1;
//}
//
////删除一条数据（前端有）
//bool DiagnoseService::removeOne(const std::string& id)
//{
//	diagnoseDO data;
//	data.setIsDelete(1);
//	data.setUpdateTime(SimpleDateTimeFormat::format());
//
//	diagnoseDAO dao;
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
//	diagnoseDAO dao;
//	return dao.deleteByIds<diagnoseDO>(dataIds) == ids->size();
//}
//
//bool DiagnoseService::batchChangeStatus(const DiagnoseBatchStatusDTO::Wrapper& dto,oatpp::String status)
//{
//	const oatpp::List<DiagnoseBatchDTO::Wrapper> ids = dto->ids;
//	diagnoseDAO dao;
//	int successCount = 0;
//	for (const auto& idItem : *ids.get())
//	{
//		diagnoseDO data;
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
//	diagnoseDAO dao;
//	int successCount = 0;
//	for (const auto& idItem : *ids.get())//返回原生的vector并枚举遍历，get获取裸指针
//	{
//		diagnoseDO data;
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
	diagnoseDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		diagnoseName, name,
		diagnoseType, diagType,
		diagnoseCode, chronicCode,
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

	DiagnoseDAO dao;
	return dao.insert(data) == 1 ? data.getId() : "";
}

bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
{
	diagnoseDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		diagnoseName, name,
		diagnoseType, diagType,
		diagnoseCode, chronicCode,
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
	DiagnoseDAO dao;
	return dao.update(data) == 1;
}

//删除一条数据（前端有）
bool DiagnoseService::removeOne(const std::string& id)
{
	diagnoseDO data;
	data.setIsDelete(1);
	data.setUpdateTime(SimpleDateTimeFormat::format());

	DiagnoseDAO dao;
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

	DiagnoseDAO dao;
	return dao.deleteByIds<DiagnoseDO>(dataIds) == ids->size();
}

//逻辑批量删除(不记录删除人)
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())//返回原生的vector并枚举遍历，get获取裸指针
	{
		diagnoseDO data;
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
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())
	{
		diagnoseDO data;
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
	DiagnoseDAO dao;
	int successCount = 0;
	for (const auto& idItem : *ids.get())//返回原生的vector并枚举遍历，get获取裸指针
	{
		diagnoseDO data;
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
