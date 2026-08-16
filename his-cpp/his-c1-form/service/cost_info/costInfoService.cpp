#include "stdafx.h"
#include "costInfoService.h"
#include "../../dao/cost_info/costInfoDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"


CostInfoDataDTO::Wrapper CostInfoService::listAll(const CostInfoQuery::Wrapper& query) {
	// 获取返回对象
	auto pages = CostInfoDataDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 获取数据量
	CostInfoDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页
	pages->total = count;
	pages->calcPages();
	// full-summary over all rows, charge add and refund subtract
	pages->summary = dao.calcuSummary(query);
	list<CostInfoDO> result = dao.selectWithPage(query);

	for (CostInfoDO& sub : result)
	{
		auto dto = CostInfoDTO::createShared();

		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			chargeID, ChargeID, encounterID, EncounterID, paymentID, PaymentID,
			cashierID, CashierID, name, Name, room, Room, roomID, RoomID,
			date, Date, feeClass, FeeClass, feeClassCode, FeeClassCode, detailClass, DetailClass, detailClassCode, DetailClassCode,
			projectName, ProjectName, hisID, HisID, spec, Spec,
			number, Number, unit, Unit, doctor, Doctor, checker, Checker);
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			settleTime, SettleTime, source, Source,
			insuranceLevel, InsuranceLevel, insuranceType, InsuranceType,
			originalPrice, OriginalPrice, discountAmount, DiscountAmount,
			receivableAmount, ReceivableAmount, discountedPrice, DiscountedPrice,
			unitPrice, UnitPrice, lineAmount, LineAmount, actualAmount, ActualAmount,
			insuranceAmount, InsuranceAmount, selfPayAmount, SelfPayAmount);

		pages->addData(dto);
	}

	return pages;
}

CostInfoDataDTO::Wrapper CostInfoService::exportAll(const CostInfoQuery::Wrapper& query)
{
	// 获取返回对象
	auto pages = CostInfoDataDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 获取数据量
	CostInfoDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}
	
	// 获取全部数据
	pages->total = count;
	pages->calcPages();
	// full-summary over all rows, charge add and refund subtract
	pages->summary = dao.calcuSummary(query);
	list<CostInfoDO> result = dao.selectAll(query);

	for (CostInfoDO& sub : result)
	{
		auto dto = CostInfoDTO::createShared();

		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			chargeID, ChargeID, encounterID, EncounterID, paymentID, PaymentID,
			cashierID, CashierID, name, Name, room, Room, roomID, RoomID,
			date, Date, feeClass, FeeClass, feeClassCode, FeeClassCode, detailClass, DetailClass, detailClassCode, DetailClassCode,
			projectName, ProjectName, hisID, HisID, spec, Spec,
			number, Number, unit, Unit, doctor, Doctor, checker, Checker,
			settleTime, SettleTime, source, Source);

		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
			insuranceLevel, InsuranceLevel, insuranceType, InsuranceType,
			originalPrice, OriginalPrice, discountAmount, DiscountAmount,
			receivableAmount, ReceivableAmount, discountedPrice, DiscountedPrice,
			unitPrice, UnitPrice, lineAmount, LineAmount, actualAmount, ActualAmount,
			insuranceAmount, InsuranceAmount, selfPayAmount, SelfPayAmount);

		pages->addData(dto);
	}

	return pages;
}

