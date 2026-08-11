#pragma once

#ifndef _SETTLE_CHARGE_DAO_H_
#define _SETTLE_CHARGE_DAO_H_

#include "BaseDAO.h"

/**
 * 结算写操作 DAO（接口3C）
 */
class SettleChargeDAO : public BaseDAO
{
public:
    /**
     * 根据 encounterId 查询真实 patientId
     */
    std::string selectPatientIdByEncounter(const std::string& encounterId);

    /**
     * 插入支付流水记录
     */
    void insertPayment(const std::string& paymentId, const std::string& nowStr,
        const std::string& paymentNo, const std::string& entererId,
        const std::string& patientId, const std::string& totalAmountStr,
        const std::string& chargeItemIdsStr,
        const std::string& billDate, const std::string& invoiceId,
        const std::string& encounterId);

    /**
     * 插入发票草稿记录
     */
    void insertInvoice(const std::string& invoiceId, const std::string& nowStr,
        const std::string& encounterId, const std::string& paymentId,
        const std::string& chargeItemIdsStr, const std::string& totalAmountStr,
        const std::string& busNo);

    /**
     * 更新指定收费项状态为已结算
     */
    void updateChargeItemsToSettled(const std::string& chargeItemIdsStr,
        const std::string& nowStr);
};

#endif // !_SETTLE_CHARGE_DAO_H_
