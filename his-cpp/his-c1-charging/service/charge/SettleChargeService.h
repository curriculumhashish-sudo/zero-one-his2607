#pragma once

#ifndef _SETTLE_CHARGE_SERVICE_H_
#define _SETTLE_CHARGE_SERVICE_H_

#include "domain/vo/charge/SettleChargeVO.h"
#include "domain/dto/charge/PreSettleChargeDTO.h"
#include "domain/dto/charge/ConfirmSettleChargeDTO.h"

/**
 * 结算服务（接口3A预结算 + 接口3C正式结算）
 */
class SettleChargeService
{
public:
    /**
     * 预结算：生成支付草稿，锁定收费项，按费用性质分组
     * @param dto 请求参数（encounterId + chargeItemIds）
     * @return 预结算响应（paymentDraftId, displayAmount, previews, selfPayRollup, insuranceGroups）
     */
    PreSettleChargeResponseDTO::Wrapper preSettle(const PreSettleChargeRequestDTO::Wrapper& dto);

    /**
     * 正式结算：落账收费，写入支付流水和发票
     * @param dto 请求参数（paymentDraftId, encounterId, chargeItemIds, paymentDetails 等）
     * @return 正式结算响应（paymentId, invoiceId, invoiceStatus 等）
     */
    ConfirmSettleChargeResponseDTO::Wrapper confirmSettle(const ConfirmSettleChargeRequestDTO::Wrapper& dto);
};

#endif // !_SETTLE_CHARGE_SERVICE_H_
