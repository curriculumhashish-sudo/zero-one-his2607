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
#include "TransactionManager.h"

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
		auto dto = TreatmentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, code, BusNo, status, StatusEnum, classification, CategoryCode);
	}
	return pages;
}

// 下拉/选择列表查询（仅 id + name）
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

// 从新增DTO填充主表业务字段（不含主键/审计）
static void fillMainFromAddDto(TreatmentDO& data, const TreatmentAddDTO::Wrapper& dto)
{
	if (dto->name) data.setName(dto->name.getValue(""));
	if (dto->categoryCode) data.setCategoryCode(dto->categoryCode.getValue(""));
	if (dto->typeEnum) data.setTypeEnum(std::to_string(dto->typeEnum.getValue(0)));
	if (dto->packageUnitCode) data.setPackageUnitCode(dto->packageUnitCode.getValue(""));
	if (dto->pyStr) data.setPyStr(dto->pyStr.getValue(""));
	if (dto->wbStr) data.setWbStr(dto->wbStr.getValue(""));
	if (dto->descriptionText) data.setDescriptionText(dto->descriptionText.getValue(""));
	if (dto->ybFlag) data.setYbFlag(dto->ybFlag.getValue(0));
	if (dto->ybNo) data.setYbNo(dto->ybNo.getValue(""));
	if (dto->ybMatchFlag) data.setYbMatchFlag(dto->ybMatchFlag.getValue(0));
	if (dto->chrgitmLv) data.setChrgitmLv(dto->chrgitmLv.getValue(0));

	bool hasChildren = dto->children && dto->children->size() > 0;
	data.setChildrenFlag(hasChildren ? 1 : 0);
}

// 组装并插入子项列表
static bool insertChildren(TreatmentDAO& dao, const std::string& activityId,
	const oatpp::List<TreatmentChildDTO::Wrapper>& children,
	const std::string& username, const std::string& now)
{
	if (!children || children->size() == 0)
		return true;

	UuidFacade uf;
	for (auto childDto : *children.get())
	{
		TreatmentChildDO child;
		child.setId(uf.genUuid());
		child.setActivityId(activityId);
		if (childDto->childrenId) child.setChildrenId(childDto->childrenId.getValue(""));
		if (childDto->childrenName) child.setChildrenName(childDto->childrenName.getValue(""));
		if (childDto->childrenNum) child.setChildrenNum(childDto->childrenNum.getValue(0));
		child.setCreatedBy(username);
		child.setCreatedAt(now);
		child.setIsDeleted(0);
		child.setVersion(0);
		if (dao.insert(child) != 1)
			return false;
	}
	return true;
}

// 通过ID查询诊疗详情（对齐 SampleService::getById）
TreatmentDetailDTO::Wrapper TreatmentService::getById(std::string id)
{
	TreatmentDAO dao;
	auto res = dao.selectById(id);
	if (!res)
		return nullptr;

	auto dto = TreatmentDetailDTO::createShared();
	dto->id = res->getId();
	dto->code = res->getBusNo();
	dto->name = res->getName();
	dto->status = res->getStatusEnum();
	dto->classification = res->getCategoryCode();
	// 价格表 DO 尚未接入，先返回空字符串
	dto->price = "";
	return dto;
}

// 新增诊疗：主表 + 子项（价格表后续补）
std::string TreatmentService::saveData(const TreatmentAddDTO::Wrapper& dto)
{
	TreatmentDAO dao;
	UuidFacade uf;
	std::string id = uf.genUuid();
	std::string now = SimpleDateTimeFormat::format();
	std::string username = dto->getPayload() ? dto->getPayload()->getUsername() : "";

	TreatmentDO data;
	data.setId(id);
	// 业务编码暂用主键，后续若有独立编码规则再改
	data.setBusNo(id);
	data.setStatusEnum("ACTIVE");
	data.setIsDeleted(0);
	data.setVersion(0);
	data.setCreatedBy(username);
	data.setCreatedAt(now);
	fillMainFromAddDto(data, dto);

	TransactionManager tm(&dao);
	if (dao.insert(data) != 1)
	{
		tm.rollback();
		return "";
	}
	if (!insertChildren(dao, id, dto->children, username, now))
	{
		tm.rollback();
		return "";
	}
	tm.commit();
	return id;
}

// 修改诊疗：更新主表，子项先软删再插
bool TreatmentService::updateData(const TreatmentModifyDTO::Wrapper& dto)
{
	TreatmentDAO dao;
	std::string id = dto->id.getValue("");
	std::string now = SimpleDateTimeFormat::format();
	std::string username = dto->getPayload() ? dto->getPayload()->getUsername() : "";

	TreatmentDO data;
	data.setId(id);
	data.setUpdatedBy(username);
	data.setUpdatedAt(now);
	fillMainFromAddDto(data, dto);

	TransactionManager tm(&dao);
	if (dao.update(data) != 1)
	{
		tm.rollback();
		return false;
	}
	dao.softDeleteChildrenByActivityId(id, username, now);
	if (!insertChildren(dao, id, dto->children, username, now))
	{
		tm.rollback();
		return false;
	}
	tm.commit();
	return true;
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