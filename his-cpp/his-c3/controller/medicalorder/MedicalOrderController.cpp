#include "stdafx.h"
#include "MedicalOrderController.h"
#include "../../service/medicalorder/MedicalOrderService.h"

/**
 * 签发医嘱接口
 * POST /medical-order/sign
 * 将DRAFT状态的医嘱改为ACTIVE，使其生效
 */
StringJsonVO::Wrapper MedicalOrderController::execSignAdvice(const SignAdviceDTO::Wrapper& dto, const PayloadDTO& payload) {
	auto jvo = StringJsonVO::createShared();
	if (MedicalOrderService().sign(dto))
	{
		jvo->success(nullptr);
	}
	else
	{
		jvo->fail(nullptr);
	}
	return jvo;
}

/**
 * 撤回医嘱接口
 * POST /medical-order/withdraw
 * 将ACTIVE状态的医嘱改为CANCELLED，撤回作废
 */
StringJsonVO::Wrapper MedicalOrderController::execWithdrawAdvice(const WithdrawAdviceDTO::Wrapper& dto, const PayloadDTO& payload) {
	auto jvo = StringJsonVO::createShared();
	if (MedicalOrderService().withdraw(dto))
	{
		jvo->success(nullptr);
	}
	else
	{
		jvo->fail(nullptr);
	}
	return jvo;
}

/**
 * 取消停嘱接口
 * POST /medical-order/cancel-stop
 * 将STOPPED状态的医嘱恢复为ACTIVE，重新生效
 */
StringJsonVO::Wrapper MedicalOrderController::execCancelStopAdvice(const CancelStopAdviceDTO::Wrapper& dto, const PayloadDTO& payload) {
	auto jvo = StringJsonVO::createShared();
	if (MedicalOrderService().cancelStop(dto))
	{
		jvo->success(nullptr);
	}
	else
	{
		jvo->fail(nullptr);
	}
	return jvo;
}