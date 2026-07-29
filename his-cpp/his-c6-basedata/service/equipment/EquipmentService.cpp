#include "stdafx.h"
#include "EquipmentService.h"
#include "../../dao/equipment/EquipmentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

// 列表分页
EquipmentDetailPageDTO::Wrapper EquipmentService::listAll(const EquipmentQuery::Wrapper& query)
{
	// 构建返回对象
	auto pages = EquipmentDetailPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	EquipmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<WkfInventoryItemViewDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (WkfInventoryItemViewDO& sub : result)
	{
		auto dto = EquipmentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, 
			id, Id,
			code,ProductBusNo,
			name, Name,
			classification, TotalVolume,
			specification, InventoryStatusEnum,
			manufacturer, Manufacturer
		);
		if (sub.getDeviceCategoryCode() == "1")
		{
			dto->status = 1;
		}
		else
		{
			dto->status = 0;
		}
		pages->addData(dto);
	}
	return pages;
}

// 通过id查询
EquipmentDTO::Wrapper EquipmentService::getById(std::string id)
{
	// 查询数据
	EquipmentDAO dao;
	auto res = dao.selectById(id);

	// 没有查询到数据
	if (!res)
		return nullptr;

	// 查询到数据转换成DTO
	auto dto = EquipmentDTO::createShared();
	ZO_STAR_DOMAIN_DO_TO_DTO_1(
		dto, res,
		id, Id,
		name, Name,
		classification, CategoryCode,
		pinyin, PyStr,
		wubi, WbStr
		);
	return dto;
}

// 新增器材
std::string EquipmentService::saveData(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	WkfInventoryItemDO entity;

	UuidFacade uuidFacade;
	std::string newId = uuidFacade.genUuid();
	entity.setId(newId);

	entity.setName(dto->name.getValue(""));
	entity.setCategoryCode(dto->classification.getValue(""));

	entity.setCreatedBy(payload.getId());
	entity.setCreatedAt(SimpleDateTimeFormat::format());

	// 假设默认启用
	entity.setInventoryStatusEnum("ACTIVE");

	if (dao.insert(entity) > 0) {
		return newId;
	}
	return "";
}

// 修改器材
bool EquipmentService::modifyData(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	WkfInventoryItemDO entity;

	// 使用 .getValue 获取值
	entity.setId(dto->id.getValue(""));
	entity.setName(dto->name.getValue(""));

	// 状态转换：前端传1(启用)，数据库存 "ACTIVE"
	if (dto->status.getValue(1) == 1) {
		entity.setInventoryStatusEnum("ACTIVE");
	}
	else {
		entity.setInventoryStatusEnum("INACTIVE");
	}

	entity.setUpdatedBy(payload.getId());
	entity.setUpdatedAt(SimpleDateTimeFormat::format());

	return dao.update(entity) > 0;
}

// 批量删除器材 (软删除)
bool EquipmentService::deleteData(const EquipmentDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;

	if (!dto->ids || dto->ids->empty()) {
		return false;
	}

	std::list<std::string> idList;
	for (auto& idItem : *(dto->ids)) {
		// idItem 也是 oatpp::String，取值用 .getValue
		idList.push_back(idItem.getValue(""));
	}

	std::string deletedBy = payload.getId();
	std::string deletedAt = SimpleDateTimeFormat::format();

	return dao.softDeleteByIds(idList, deletedBy, deletedAt) > 0;
}

// 批量启停用器材
bool EquipmentService::changeStatus(const EquipmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	if (!dto->ids || dto->ids->empty()) {
		return false;
	}

	std::list<std::string> idList;
	for (auto& idItem : *(dto->ids)) {
		idList.push_back(idItem.getValue(""));
	}

	// 状态转换
	std::string targetStatus = (dto->status.getValue(1) == 1) ? "ACTIVE" : "INACTIVE";
	std::string updatedBy = payload.getId();
	std::string updatedAt = SimpleDateTimeFormat::format();

	return dao.updateStatusByIds(idList, targetStatus, updatedBy, updatedAt) > 0;
}

// 选择器材 (下拉框精简版)
EquipmentSelectPageDTO::Wrapper EquipmentService::selectData(const EquipmentQuery::Wrapper& query)
{
	auto pages = EquipmentSelectPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	EquipmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) return pages;

	pages->total = count;
	pages->calcPages();

	list<WkfInventoryItemViewDO> result = dao.selectWithPage(query);
	for (WkfInventoryItemViewDO& sub : result)
	{
		auto dto = EquipmentSelectDTO::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		// dto->specification = sub.getSpecification();
		pages->addData(dto);
	}
	return pages;
}