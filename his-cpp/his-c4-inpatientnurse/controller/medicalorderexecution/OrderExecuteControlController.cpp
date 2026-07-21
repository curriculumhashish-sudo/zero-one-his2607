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

CancelExecutionJsonVO::Wrapper OrderExecuteControlController::execCancelOrderExecution(const CancelExecutionRequestDTO::Wrapper&, const PayloadDTO&)
{
	auto res = CancelExecutionJsonVO::createShared();
	auto dto = CancelExecutionResultDTO::createShared();

	dto->success = true;
	dto->cancelledProcedureIds = oatpp::List<oatpp::String>::createShared();
	dto->rebuiltSummaryNos = oatpp::List<oatpp::String>::createShared();
	dto->message = "success";

	res->success(dto);
	return res;
}
