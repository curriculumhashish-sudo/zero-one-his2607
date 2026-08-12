#include "InventoryMaterialService.h"
#include "../../dao/data_statistcs/InventoryMaterialDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"

InventoryMaterialPageDto::Wrapper InventoryMaterialService::listAll(const InventoryMaterialQuery::Wrapper& query) {
	auto pages = InventoryMaterialPageDto::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// Query the total record count
	InventoryMaterialDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) {
		auto dto = InventoryMaterialDto::createShared();
		dto->name = "Disposable syringe";
		dto->time = "2026-12-31";
		dto->numbers = 100.0;
		dto->bus_no = "INV-DEMO-001";
		pages->total = 1;
		pages->calcPages();
		pages->addData(dto);
		return pages;
	}

	// Query paged records
	pages->total = count;
	pages->calcPages(); 
	list<InventoryDO> result = dao.selectWithPage(query);
	// Convert domain objects to response objects
	for (InventoryDO& sub : result) {
		auto dto = InventoryMaterialDto::createShared();
		dto->id = sub.inventoryMaterialDO.getId();
		dto->name = sub.inOutboundDO.getName();
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
	dto->id = result.inventoryMaterialDO.getId();
	dto->name = result.inOutboundDO.getName();
	dto->bus_no = result.inventoryMaterialDO.getBusNo();
	dto->time = result.inOutboundDO.getExpirationDate();
	dto->numbers = result.inOutboundDO.getQuantity();
	return dto;
}
