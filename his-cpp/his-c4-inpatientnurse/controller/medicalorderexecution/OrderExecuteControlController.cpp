#include "stdafx.h"
#include "OrderExecuteControlController.h"
#include "../../service/medicalorderexecution/OrderExecuteControlService.h"
#include "dao/common/NurseStationAccessDAO.h"

StringJsonVO::Wrapper OrderExecuteControlController::execSkipOrderExecution(const List<OrderNotExecuteDTO::Wrapper>& dtolist, const PayloadDTO& payload)
{
	auto res = StringJsonVO::createShared();
	if (!dtolist || dtolist->size() == 0 || dtolist->size() > 50) {
		res->init(ZH_WORDS_GETTER("medical_advice.notexecute.failreason.listnotvalid"), RS_PARAMS_INVALID);
		return res;
	}
	for (auto& dto : *dtolist)
	{
		auto checkinfo = OrderNotExecuteDTO::validate(dto);
		if (!checkinfo.empty()) {
			res->init(checkinfo, RS_PARAMS_INVALID);
			return res;
		}
	}

	string service_res = OrderExecuteControlService().skipOrderExecution(dtolist, payload);
	if (service_res.empty())
		res->success("success");
	else
		res->init(service_res, RS_FAIL);

	return res;
}

CancelExecutionJsonVO::Wrapper OrderExecuteControlController::execCancelOrderExecution(const CancelExecutionRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto res = CancelExecutionJsonVO::createShared();

	if (!dto)
	{
		auto result = CancelExecutionResultDTO::createShared();
		result->success = false;
		result->message = "request body is required.";
		res->init(result, RS_PARAMS_INVALID);
		return res;
	}
	auto checkinfo = dto->validate();
	if (!checkinfo.empty())
	{
		auto result = CancelExecutionResultDTO::createShared();
		result->success = false;
		result->message = checkinfo;
		res->init(result, RS_PARAMS_INVALID);
		return res;
	}

	auto result = OrderExecuteControlService().cancelOrderExecution(dto, payload);


	res->success(result);
	return res;
}

BatchOrderExecutionJsonVO::Wrapper
OrderExecuteControlController::execSkipOrderExecutionBatch(
	const BatchSkipOrderRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = BatchOrderExecutionJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| !dto->items || dto->items->empty() || dto->items->size() > 50
		|| (inpatientnurse::validation::present(dto->reason) && dto->reason->size() > 500)) {
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> unique;
	for (const auto& item : *dto->items) {
		if (!item || !inpatientnurse::validation::present(item->orderId)
			|| !inpatientnurse::validation::oneOf(
				item->orderType, {"MED_REQUEST", "SERVICE_REQUEST"}, false)
			|| !inpatientnurse::validation::validDateTime(item->planTime, false)
			|| !unique.insert(item->orderType.getValue("") + ":"
				+ item->orderId.getValue("") + ":" + item->planTime.getValue("")).second) {
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
	}
	auto result = OrderExecuteControlService().skipBatch(dto, payload);
	result->success ? vo->success(result) : vo->fail(result);
	return vo;
}

BatchOrderExecutionJsonVO::Wrapper
OrderExecuteControlController::execCancelOrderExecutionBatch(
	const BatchCancelExecutionRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = BatchOrderExecutionJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| !inpatientnurse::validation::present(dto->reason) || dto->reason->size() > 500
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) {
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> unique;
	for (const auto& item : *dto->items) {
		if (!item || !inpatientnurse::validation::present(item->encounterId)
			|| !inpatientnurse::validation::present(item->procedureId)
			|| !unique.insert(item->procedureId.getValue("")).second) {
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
		if (!NurseStationAccessDAO().canAccessEncounter(
			payload.getId(), item->encounterId.getValue(""))) {
			vo->init(nullptr, RS_UNAUTHORIZED);
			return vo;
		}
	}
	auto result = OrderExecuteControlService().cancelBatch(dto, payload);
	result->success ? vo->success(result) : vo->fail(result);
	return vo;
}
