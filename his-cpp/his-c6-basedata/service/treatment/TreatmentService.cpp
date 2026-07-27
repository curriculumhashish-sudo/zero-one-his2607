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
// 列表分页
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

// 删除
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
