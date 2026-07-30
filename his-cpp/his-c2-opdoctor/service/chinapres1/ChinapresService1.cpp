#include "stdafx.h"
#include "ChinapresService1.h"
#include "dao/chinapres1/ChinapresDAO1.h"
#include "dao/chinapres1/MedicationDefinitionDAO1.h"
#include "dao/chinapres1/ChargeItemDefinitionDAO1.h"
#include <map>

ChinapresPageDTO1::Wrapper ChinapresService1::queryList(const ChinapresQuery1::Wrapper& query)
{
	auto page = ChinapresPageDTO1::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	ChinapresDAO1 dao;
	auto count = dao.count(query);
	if (count <= 0) {
		return page;
	}
	page->total = count;//总条数
	page->calcPages();//总页数
	
	auto preList = dao.selectAll(query);

	std::map<std::string, std::list<PtrChinapresDO>> groupMap;
	for (auto one: preList) {
		groupMap[one->getPrescriptionNo()].push_back(one);
	}
	MedicationDefinitionDAO1 medDao;

	for (auto pair : groupMap) {
		auto presNo = pair.first;
		auto records = pair.second;

		auto dto = ChinapresDTO1::createShared();
		dto->id = presNo;
		dto->count = std::to_string(records.size());

		std::string herbsStr;
		for (auto record : records) {
			auto medDef = medDao.selectById(record->getMedicationId());
			if (medDef && !medDef->getName().empty()) {
				if (!herbsStr.empty()) {
					herbsStr += ",";
				}
				herbsStr += medDef->getName();
			}
		}
		dto->herbs = herbsStr;
		page->addData(dto);
	}
	return page;
}

ChinapresDetailDTO1::Wrapper ChinapresService1::queryDetail(const ChinapresDetailQuery1::Wrapper& query)
{
	ChinapresDAO1 dao;
	auto detailList = dao.selectDetail(query);
	if (detailList.empty()) {
		return nullptr;
	}
	auto first = detailList.front();
	auto dto = ChinapresDetailDTO1::createShared();
	dto->presCno = first->getPrescriptionNo();
	dto->status = first->getStatusEnum();
	dto->usage = first->getMethodCode();
	dto->frequency = first->getRateCode();
	dto->doses = (oatpp::Int32)(int)first->getChineseHerbsDoseQuantity();
	dto->days = (oatpp::Int32)first->getExecuteNum();
	dto->decoct = first->getSufferingFlag();
	dto->diagnosis = first->getConditionDefinitionId();
	dto->remark = first->getRemark();
	dto->totalcount = (oatpp::Int32)(int)detailList.size();

	MedicationDefinitionDAO1 medDao;
	ChargeItemDefinitionDAO1 chargeDao;
	auto herbList = oatpp::List<oatpp::Object<ChinapresHerbDTO>>::createShared();
	double totalDoseAmount = 0.0;
	for (auto record : detailList) {
		auto herb = ChinapresHerbDTO::createShared();

		auto medDef = medDao.selectById(record->getMedicationId());
		herb->herbName = medDef ? medDef->getName() : "未知药材";
		//单价
		auto chargeDef = chargeDao.selectByInstanceId(record->getMedicationId());
		double unitPrice = chargeDef ? chargeDef->getPriceValue() : 0.0;
		herb->unitprice = unitPrice;
		//剂量加单位
		herb->dosage = std::to_string((int)record->getDose()) + record->getDoseUnitCode();
		//总数
		double subtotal = unitPrice * record->getDose() * first->getChineseHerbsDoseQuantity();
		herb->subtotal = subtotal;
		//累加单剂金额
		totalDoseAmount += unitPrice * record->getDose();
		herbList->push_back(herb);
	}
	dto->details = herbList;
	dto->doseAmount = std::round(totalDoseAmount * 100) / 100;
	dto->totalAmount = std::round(totalDoseAmount * first->getChineseHerbsDoseQuantity() * 100) / 100;

	return dto;
}
