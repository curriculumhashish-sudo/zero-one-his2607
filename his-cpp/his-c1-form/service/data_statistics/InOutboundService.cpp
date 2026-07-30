#include "InOutboundService.h"
#include "../../dao/data_statistcs/InOutboundDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"

InOutboundPageDto::Wrapper InOutboundService::listAll(const InOutboundQuery::Wrapper& query) {
	auto pages = InOutboundPageDto::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	InOutboundDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) {
		return pages;
	}

	pages->total = count;
	pages->calcPages();

	list<InOutboundDO> result = dao.selectWithPage(query);
	for (InOutboundDO& sub : result) {
		auto dto = InOutboundDto::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		dto->onemoney = sub.getLotNumberCostAmount();
		dto->allmoney = sub.getLineCostTotal();
		dto->outmoney = sub.getLotNumberPriceAmount();
		dto->alloutmoney = sub.getLinePriceTotal();
		dto->batch_number = sub.getLotNumber();
		pages->addData(dto);
	}
	return pages;
}

InOutboundDto::Wrapper InOutboundService::getById(std::string id) {
	InOutboundDAO dao;
	InOutboundDO result = dao.selectById(id);
	if (result.getId().empty()) {
		return nullptr;
	}
	auto dto = InOutboundDto::createShared();
	dto->id = result.getId();
	dto->name = result.getName();
	dto->onemoney = result.getLotNumberCostAmount();
	dto->allmoney = result.getLineCostTotal();
	dto->outmoney = result.getLotNumberPriceAmount();
	dto->alloutmoney = result.getLinePriceTotal();
	dto->batch_number = result.getLotNumber();
	return dto;
}
