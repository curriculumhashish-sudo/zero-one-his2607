#include "stdafx.h"
#include "DeptBatchStatusController.h"

// 先保留头文件引入，后续写完Service再取消注释
//#include "service/dept_batch_status_service.hpp"

DeptBatchStatusJsonVO::Wrapper DeptController::execBatchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	std::string errMsg = dto->validate();
	if (!errMsg.empty())
	{
		auto vo = DeptBatchStatusJsonVO::createShared();
		// 替换setStatus，改用init
		vo->init(RS_FAIL);
		vo->message = errMsg;
		return vo;
	}

	dto->setPayload(&payload);
	bool opSuccess = true;

	auto respVo = DeptBatchStatusJsonVO::createShared();
	if (opSuccess)
	{
		// 成功状态
		respVo->init(RS_SUCCESS);
		if (dto->targetStatus == 1)
			respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.enable");
		else
			respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.disable");
	}
	else
	{
		// 失败状态
		respVo->init(RS_FAIL);
		respVo->message = ZH_WORDS_GETTER("dept.batch-status.fail.empty");
	}
	return respVo;
}