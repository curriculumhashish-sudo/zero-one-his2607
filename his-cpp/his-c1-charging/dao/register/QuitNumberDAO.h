#pragma once
#ifndef _QUIT_NUMBER_DAO_
#define _QUIT_NUMBER_DAO_

#include "BaseDAO.h"
#include <string>

/**
 * 确认退号数据访问对象
 * 职责：执行退号相关的数据库更新和插入操作
 * 
 */
class QuitNumberDAO : public BaseDAO {
public:
    // --------------------- 更新操作 ---------------------

    /**
     * 更新就诊记录状态：REGISTERED → CANCELLED
     * @param id 就诊记录ID
     * @param version 乐观锁版本号
     */
    int updateEncounterToCancelled(const std::string& id, int version);

    /**
     * 标记候诊队列记录为取消
     * @param encounterId 就诊记录ID
     * @param cancelReason 取消原因文本
     */
    int updateQueueToCancelled(const std::string& encounterId, const std::string& cancelReason);

    /**
     * 释放号源
     * @param slotId 号源ID
     */
    int releaseSlot(const std::string& slotId);

    /**
     * 更新收费项目状态：PAID → REFUNDED
     * @param chargeItemId 收费项目ID
     * @param refundId 退款记录ID
     * @param version 乐观锁版本号
     */
    int updateChargeItemToRefunded(const std::string& chargeItemId, const std::string& refundId, int version);

    /**
     * 插入退款支付对账记录
     * @note 参数较多，实际项目中可考虑定义结构体传参，此处保持接口简单
     */
    int insertRefundPayment(const std::string& id, const std::string& patientId, const std::string& encounterId,
        const std::string& paymentNo, const std::string& paymentEnum, double amount,
        const std::string& chargeItemIds, const std::string& entererId);

    /**
     * 插入退款支付明细记录
     */
    int insertRefundDetail(const std::string& id, const std::string& reconciliationId,
        const std::string& payEnum, double returnAmount);

    /**
     * 更新发票状态：ISSUED → CANCELLED
     * @param invoiceId 发票ID
     * @param cancelledReason 作废原因
     */
    int updateInvoiceToCancelled(const std::string& invoiceId, const std::string& cancelledReason);

    /**
     * 退还账户余额
     * @param accountId 账户ID
     * @param amount 退款金额
     */
    int updateAccountBalanceRefund(const std::string& accountId, double amount);
};

#endif // !_QUIT_NUMBER_DAO_
