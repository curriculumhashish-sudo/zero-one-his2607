#include "stdafx.h"
#include "OrderExecuteControlController.h"

StringJsonVO::Wrapper OrderExecuteControlController::execSkipOrderExecution(const List<OrderNotExecuteDTO::Wrapper>& dtolist, const PayloadDTO& payload)
{
	auto res = StringJsonVO::createShared();
	//检查整个列表参数 TODO - 这里没有检查列表参数不能>50，后面评估是否需要添加
	if (!dtolist || dtolist->size() == 0) {
		res->init(ZH_WORDS_GETTER("medical_advice.notexecute.failreason.listnotvalid"), RS_PARAMS_INVALID);
		return res;
	}
	//遍历每条不执行医嘱DTO，检查参数是否合法
	for (auto& dto : *dtolist)
	{
		string checkinfo;
		if (!dto){
			res->init(ZH_WORDS_GETTER("medical_advice.notexecute.failreason.partparainvalid"), RS_PARAMS_INVALID);
			return res;
		}
		if (!(checkinfo = dto->validate()).empty()){
			res->init(checkinfo, RS_PARAMS_INVALID);
			return res;
		}
	}

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
