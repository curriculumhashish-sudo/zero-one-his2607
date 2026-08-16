#include "stdafx.h"
#include "MedicalOrderController.h"
#include "../../service/medicalorder/MedicalOrderService.h"

namespace {
// 业务层返回空对象表示操作失败，这里统一转换成接口失败响应。
MedicalOrderOperationResultVO::Wrapper makeResponse(const MedicalOrderOperationResultDTO::Wrapper& data)
{
    auto vo = MedicalOrderOperationResultVO::createShared();
    if (!data) {
        vo->fail(nullptr);
        return vo;
    }
    vo->success(data);
    return vo;
}
}

MedicalOrderOperationResultVO::Wrapper MedicalOrderController::execSignAdvice(const SignAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    return makeResponse(MedicalOrderService().sign(dto, payload));
}

MedicalOrderOperationResultVO::Wrapper MedicalOrderController::execWithdrawAdvice(const WithdrawAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    return makeResponse(MedicalOrderService().withdraw(dto, payload));
}

MedicalOrderOperationResultVO::Wrapper MedicalOrderController::execCancelStopAdvice(const CancelStopAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    return makeResponse(MedicalOrderService().cancelStop(dto, payload));
}
