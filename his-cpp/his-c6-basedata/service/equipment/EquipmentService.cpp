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
	list<WkfInventoryItemDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (WkfInventoryItemDO& sub : result)
	{
		auto dto = EquipmentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, name, Name);
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

std::string EquipmentService::saveData(const EquipmentAddDTO::Wrapper& dto)
{
	return {};
}

// 修改数据
bool modifyData(const EquipmentModifyDTO::Wrapper& dto)
{
	return {};
}
// 批量删除
bool deleteData(const EquipmentDeleteDTO::Wrapper& dto)
{
	return {};
}
// 批量启停用
bool changeStatus(const EquipmentStatusDTO::Wrapper& dto)
{
	return {};
}
// 列表选择查询
EquipmentSelectPageDTO::Wrapper selectData(const EquipmentQuery::Wrapper& query)
{
	return{};
}