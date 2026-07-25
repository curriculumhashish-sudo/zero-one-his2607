#pragma once

#ifndef _PRINT_CHARGE_RECEIPT_DAO_H_
#define _PRINT_CHARGE_RECEIPT_DAO_H_

#include "BaseDAO.h"
#include "domain/do/charge/PrintChargeReceiptDO.h"

/**
 * 打印相关 DAO（接口4A/4B/4C）
 */
class PrintChargeReceiptDAO : public BaseDAO
{
public:
    // 4A: 按支付ID查询小票数据
    std::list<PrintChargeReceiptSlipDO> selectSlipByPaymentId(const std::string& paymentId);

    // 4A: 按就诊ID查询最近支付的小票数据
    std::list<PrintChargeReceiptSlipDO> selectSlipByEncounterId(const std::string& encounterId);

    // 4B: 按场景码查询模板
    std::list<PrintChargeReceiptTemplateDO> selectTemplateByScenario(const std::string& scenarioCode);

    // 4C: 插入打印审计日志
    int insertPrintLog(const std::string& traceId, const std::string& busNo,
        const std::string& scenarioCode, int version, const std::string& status,
        const std::string& printData, const std::string& bizRef,
        const std::string& printer, int copies, const std::string& clientType,
        const std::string& errorCode, const std::string& errorMessage);
};

#endif // !_PRINT_CHARGE_RECEIPT_DAO_H_
