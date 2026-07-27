/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:13:11

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
#include "TreatmentService.h"
#include "../../dao/treatment/TreatmentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

// 分页列表查询
TreatmentPageDTO::Wrapper TreatmentService::listAll(const TreatmentQuery::Wrapper& query)
{
	// 构建返回对象
	auto pages = TreatmentPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;
	// 查询数据总条数
	TreatmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}
	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<TreatmentDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (TreatmentDO& sub : result)
	{
		auto dto = TreatmentDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id);
		pages->addData(dto);
	}
	return pages;
}

// 下拉/选择列表（仅 id + name）
TreatmentSelectPageDTO::Wrapper TreatmentService::listSelect(const TreatmentQuery::Wrapper& query)
{
	auto pages = TreatmentSelectPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	TreatmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	pages->total = count;
	pages->calcPages();
	list<TreatmentDO> result = dao.selectWithPage(query);
	for (TreatmentDO& sub : result)
	{
		auto dto = TreatmentSelectDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, name, Name);
		pages->addData(dto);
	}
	return pages;
}

TreatmentDetailDTO::Wrapper TreatmentService::getById(std::string id)
{
	// 空实现：后续通过 DAO/Mapper 按 id 查询并组装 TreatmentDetailDTO
	return nullptr;
}

std::string TreatmentService::saveData(const TreatmentAddDTO::Wrapper& dto)
{
	// 空实现：后续通过 DAO 写入主表/子项/价格，并返回 id
	return "";
}

bool TreatmentService::updateData(const TreatmentModifyDTO::Wrapper& dto)
{
	// 空实现：后续通过 DAO 更新主表/子项/价格
	return false;
}

// 按 ID 批量软删除
bool TreatmentService::removeData(const oatpp::List<oatpp::String>& ids)
{
	TreatmentDAO dao;
	std::list<std::string> datas;
	for (auto one : *ids.get())
		datas.emplace_back(one.getValue({}));
	int rows = dao.deleteByIds<TreatmentDO>(datas);
	if (rows == ids->size())
		return true;
	return false;
}