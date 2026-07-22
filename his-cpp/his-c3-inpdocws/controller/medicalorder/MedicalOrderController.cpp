
#include "stdafx.h"
#include "MedicalOrderController.h"

//签发医嘱
StringJsonVO::Wrapper MedicalOrderController::execSignAdvice(const SignAdviceDTO::Wrapper& dto, const PayloadDTO& payload) {
    return {};
}

//撤回医嘱
StringJsonVO::Wrapper MedicalOrderController::execWithdrawAdvice(const WithdrawAdviceDTO::Wrapper& dto, const PayloadDTO& payload) {
    return {};
}

//取消停嘱
StringJsonVO::Wrapper MedicalOrderController::execCancelStopAdvice(const CancelStopAdviceDTO::Wrapper& dto, const PayloadDTO& payload) {   
    return {};
}