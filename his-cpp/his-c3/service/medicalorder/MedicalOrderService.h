#pragma once
#include "domain/dto/medicalorder/SignAdviceDTO.h"
#include "domain/dto/medicalorder/WithdrawAdviceDTO.h"
#include "domain/dto/medicalorder/CancelStopAdviceDTO.h"
#include "domain/dto/medicalorder/MedicalOrderOperationResultDTO.h"

/**
 * 医嘱业务服务。
 * 负责签发医嘱、撤回医嘱和取消停嘱三个状态操作。
 */
class MedicalOrderService {
public:
	// 签发医嘱：DRAFT -> ACTIVE。
	MedicalOrderOperationResultDTO::Wrapper sign(const SignAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
	// 撤回医嘱：ACTIVE -> CANCELLED。
	MedicalOrderOperationResultDTO::Wrapper withdraw(const WithdrawAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
	// 取消停嘱：STOPPED -> ACTIVE。
	MedicalOrderOperationResultDTO::Wrapper cancelStop(const CancelStopAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
};
