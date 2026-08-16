#include "service/frequency/FrequencyService.h"
#include "dao/frequency/FrequencyDAO.h"
#include "domain/do/frequency/CusFrequencyDO.h"
#include "../../lib-common/include/id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

// 1. 新增频次
bool FrequencyService::addFrequency(const oatpp::Object<FrequencyDTO>& dto, std::string& errorMsg) {
	if (!dto || !dto->code || !dto->name) {
		errorMsg = "频次编码或名称不能为空";
		return false;
	}

	CusFrequencyDO data;

	// 使用组内统一宏将 DTO 属性填充至 DO 实体
	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		RateCode, code,
		Name, name,
		CycleType, cycleType,
		DayTimes, dayTimes,
		DayCount, dayCount,
		ExecutionPeriod, executionPeriod);

	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		ExecutionPeriodUnit, executionPeriodUnit,
		DayInterval, dayInterval,
		TotalExecutionCount, totalExecutionCount,
		ApplicableScenes, applicableScenes,
		AllowCrossDay, allowCrossDay,
		ActiveFlag, status,
		SortNo, sortNo);

	// 填充主键与基础字段
	UuidFacade uuidFacade;
	data.setId(uuidFacade.genUuid());
	//if (dto->getPayload()) {
	//	data.setCreatedBy(dto->getPayload()->getUsername());
	//}
	data.setCreatedAt(SimpleDateTimeFormat::format());
	data.setIsDeleted(0);

	FrequencyDAO dao;
	if (dao.insert(data) != 1) {
		errorMsg = "保存频次数据失败";
		return false;
	}

	return true;
}

// 2. 修改频次
bool FrequencyService::modifyFrequency(const oatpp::Object<FrequencyDTO>& dto, std::string& errorMsg) {
	if (!dto || !dto->id) {
		errorMsg = "记录 ID 不能为空";
		return false;
	}

	CusFrequencyDO data;
	data.setId(dto->id->c_str());

	ZO_STAR_DOMAIN_DTO_TO_DO(
		data,
		dto,
		RateCode, code,
		Name, name,
		CycleType, cycleType,
		DayTimes, dayTimes,
		DayCount, dayCount,
		ExecutionPeriod, executionPeriod,
		ExecutionPeriodUnit, executionPeriodUnit,
		DayInterval, dayInterval,
		TotalExecutionCount, totalExecutionCount,
		ApplicableScenes, applicableScenes,
		AllowCrossDay, allowCrossDay,
		ActiveFlag, status,
		SortNo, sortNo);

	//if (dto->getPayload()) {
	//	data.setUpdatedBy(dto->getPayload()->getUsername());
	//}
	data.setUpdatedAt(SimpleDateTimeFormat::format());

	FrequencyDAO dao;
	if (dao.update(data) != 1) {
		errorMsg = "修改频次数据失败";
		return false;
	}

	return true;
}

// 3. 修改启停状态
bool FrequencyService::modifyStatus(const std::string& id, int32_t targetStatus, std::string& errorMsg) {
	if (id.empty()) {
		errorMsg = "记录 ID 不能为空";
		return false;
	}

	CusFrequencyDO data;
	data.setId(id);
	data.setActiveFlag(targetStatus);
	data.setUpdatedAt(SimpleDateTimeFormat::format());

	FrequencyDAO dao;
	if (dao.update(data) != 1) {
		errorMsg = "修改状态失败";
		return false;
	}

	return true;
}

// 4. 删除频次 (逻辑删除)
bool FrequencyService::removeFrequency(const std::string& id, std::string& errorMsg) {
	if (id.empty()) {
		errorMsg = "记录 ID 不能为空";
		return false;
	}

	CusFrequencyDO data;
	data.setId(id);
	data.setIsDeleted(1);
	data.setUpdatedAt(SimpleDateTimeFormat::format());

	FrequencyDAO dao;
	if (dao.update(data) != 1) {
		errorMsg = "删除数据失败";
		return false;
	}

	return true;
}

// 5. 分页查询列表 (调用 DAO 的 count 和 selectWithPage)
FrequencyPageDTO::Wrapper FrequencyService::listAll(const FrequencyQuery::Wrapper& query) {
	auto page = FrequencyPageDTO::createShared();
	FrequencyDAO dao;

	// 1. 查询总记录数
	uint64_t totalRows = dao.count(query);

	// 2. 分页查询 DO 列表
	std::list<CusFrequencyDO> doList = dao.selectWithPage(query);

	// 3. DO 转 DTO
	auto rows = oatpp::List<FrequencyDTO::Wrapper>::createShared();
	for (const auto& item : doList) {
		auto dto = FrequencyDTO::createShared();
		dto->id = item.getId().c_str();
		dto->code = item.getRateCode().c_str();
		dto->name = item.getName().c_str();
		dto->status = item.getActiveFlag();
		dto->cycleType = item.getCycleType().c_str();
		dto->dayTimes = item.getDayTimes().c_str();
		dto->dayCount = item.getDayCount();
		dto->executionPeriod = item.getExecutionPeriod();
		dto->executionPeriodUnit = item.getExecutionPeriodUnit().c_str();
		dto->dayInterval = item.getDayInterval();
		dto->totalExecutionCount = item.getTotalExecutionCount();
		dto->applicableScenes = item.getApplicableScenes().c_str();
		dto->allowCrossDay = item.getAllowCrossDay().c_str();
		dto->sortNo = item.getSortNo();

		rows->push_back(dto);
	}
	query->pageIndex.getValue(0);
	// 4. 组装分页返回结构
	page->pageIndex = query->pageIndex.getValue(1);
	page->pageSize = query->pageSize.getValue(10);
	page->total = totalRows;
	page->rows = rows;

	return page;
}

// 6. 详情查询 (调用 DAO 的 selectById)
FrequencyDTO::Wrapper FrequencyService::getById(const std::string& id) {
	FrequencyDAO dao;
	PtrCusFrequencyDO doPtr = dao.selectById(id);
	if (!doPtr) return nullptr;

	auto dto = FrequencyDTO::createShared();
	dto->id = doPtr->getId().c_str();
	dto->code = doPtr->getRateCode().c_str();
	dto->name = doPtr->getName().c_str();
	dto->status = doPtr->getActiveFlag();
	dto->cycleType = doPtr->getCycleType().c_str();
	dto->dayTimes = doPtr->getDayTimes().c_str();
	dto->dayCount = doPtr->getDayCount();
	dto->executionPeriod = doPtr->getExecutionPeriod();
	dto->executionPeriodUnit = doPtr->getExecutionPeriodUnit().c_str();
	dto->dayInterval = doPtr->getDayInterval();
	dto->totalExecutionCount = doPtr->getTotalExecutionCount();
	dto->applicableScenes = doPtr->getApplicableScenes().c_str();
	dto->allowCrossDay = doPtr->getAllowCrossDay().c_str();
	dto->sortNo = doPtr->getSortNo();

	return dto;
}

// 7. 下拉框选择列表 (调用 DAO 的 selectByName，转为 FrequencyVO)
oatpp::List<FrequencyVO::Wrapper> FrequencyService::getName(const FrequencyQuery::Wrapper& query) {
	auto voList = oatpp::List<FrequencyVO::Wrapper>::createShared();
	FrequencyDAO dao;

	std::string searchName = (query && query->keywords) ? query->keywords->c_str() : "";
	std::list<CusFrequencyDO> doList = dao.selectByName(searchName);

	for (const auto& item : doList) {
		auto vo = FrequencyVO::createShared();
		vo->id = item.getId().c_str();
		vo->name = item.getName().c_str();
		voList->push_back(vo);
	}

	return voList;
}