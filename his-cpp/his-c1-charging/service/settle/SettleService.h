#pragma once

#ifndef _SETTLE_SERVICE_H_
#define _SETTLE_SERVICE_H_

#include "domain/dto/PayloadDTO.h"
#include "domain/dto/settle/SettleDTO.h"

// 住院结算业务服务
class SettleService {
public:
	// 在同一事务中完成费用校验、支付落库和结算状态更新
	SettleResultDTO::Wrapper settle(const SettleRequestDTO::Wrapper &dto, const PayloadDTO &payload);
};

#endif
