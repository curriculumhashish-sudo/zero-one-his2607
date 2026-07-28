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
		dto->allmoney = sub.getLotNumberPriceAmount();
		dto->alloutmoney = sub.getLotNumberCostAmount();
		dto->batch_number = sub.getBusNo();
		dto->name = sub.getName();
		dto->onemoney = sub.getLinePriceTotal();
		dto->outmoney = sub.getLineCostTotal();
		pages->addData(dto);
	}
	return pages;
}

InOutboundDto::Wrapper InOutboundService::getById(std::string id) {
	InOutboundDAO dao;
	InOutboundDO result = dao.selectById(id);
	if (result.getBusNo().empty()) {
		return nullptr;
	}
	auto dto = InOutboundDto::createShared();
	dto->allmoney = result.getLotNumberPriceAmount();
	dto->alloutmoney = result.getLotNumberCostAmount();
	dto->batch_number = result.getBusNo();
	dto->name = result.getName();
	dto->onemoney = result.getLinePriceTotal();
	dto->outmoney = result.getLineCostTotal();
	return dto;
}