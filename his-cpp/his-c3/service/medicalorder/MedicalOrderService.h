#pragma once
#include "domain/dto/medicalorder/SignAdviceDTO.h"
#include "domain/dto/medicalorder/WithdrawAdviceDTO.h"
#include "domain/dto/medicalorder/CancelStopAdviceDTO.h"

/**
 * 医嘱业务服务
 * 实现签发、撤回、取消停嘱三个核心业务操作
 */
class MedicalOrderService {
public:
	// 签发医嘱：将DRAFT（待签发）状态的医嘱改为ACTIVE（生效中）
	bool sign(const SignAdviceDTO::Wrapper& dto);
	// 撤回医嘱：将ACTIVE（生效中）状态的医嘱改为CANCELLED（已撤回）
	bool withdraw(const WithdrawAdviceDTO::Wrapper& dto);
	// 取消停嘱：将STOPPED（已停嘱）状态的医嘱恢复为ACTIVE（生效中）
	bool cancelStop(const CancelStopAdviceDTO::Wrapper& dto);
};