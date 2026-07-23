#include "stdafx.h"
#include "OrderExecuteControlController.h"
#include "../../service/medicalorderexecution/OrderExecuteControlService.h"

StringJsonVO::Wrapper OrderExecuteControlController::execSkipOrderExecution(const List<OrderNotExecuteDTO::Wrapper>& dtolist, const PayloadDTO& payload)
{
	auto res = StringJsonVO::createShared();
	//检查整个列表参数，非空，列表长度不能>50
	if (!dtolist || dtolist->size() == 0 || dtolist->size() > 50) {
		res->init(ZH_WORDS_GETTER("medical_advice.notexecute.failreason.listnotvalid"), RS_PARAMS_INVALID);
		return res;
	}
	//遍历每条不执行医嘱DTO，检查参数是否合法
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

	//基础数据校验
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

	//调用业务层函数
	auto result = OrderExecuteControlService().cancelOrderExecution(dto, payload);


	res->success(result);
	return res;
}
