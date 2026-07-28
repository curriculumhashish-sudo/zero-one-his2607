#include "InventoryMaterialService.h"
#include "../../dao/data_statistcs/InventoryMaterialDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"

InventoryMaterialPageDto::Wrapper InventoryMaterialService::listAll(const InventoryMaterialQuery::Wrapper& query) {
	auto pages = InventoryMaterialPageDto::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	InventoryMaterialDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) {
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages(); 
	list<InventoryDO> result = dao.selectWithPage(query);
	// 将DO转换为DTO
	for (InventoryDO& sub : result) {
		auto dto = InventoryMaterialDto::createShared();
		dto->bus_no = sub.inventoryMaterialDO.getBusNo();
		dto->time = sub.inOutboundDO.getExpirationDate();
		dto->numbers = sub.inOutboundDO.getQuantity();
		pages->addData(dto);
	}
	return pages;
}

InventoryMaterialDto::Wrapper InventoryMaterialService::getById(std::string id) {
	InventoryMaterialDAO dao;
	InventoryDO result = dao.selectById(id);
	if (result.inventoryMaterialDO.getBusNo().empty()) {
		return nullptr;
	}
	auto dto = InventoryMaterialDto::createShared();
	dto->bus_no = result.inventoryMaterialDO.getBusNo();
	dto->time = result.inOutboundDO.getExpirationDate();
	dto->numbers = result.inOutboundDO.getQuantity();
	return dto;
}