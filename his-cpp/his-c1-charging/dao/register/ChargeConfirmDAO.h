#pragma once
#ifndef _CHARGE_CONFIRM_DAO_
#define _CHARGE_CONFIRM_DAO_
#include "BaseDAO.h"  // 数据访问基类，提供sqlSession等工具

/**
 * 确认缴费业务数据访问对象
 * 职责：处理确认缴费相关的数据库操作（状态更新、记录插入）
 *
 * 操作表清单：
 * - fin_payment_reconciliation: 支付对账表（草稿转正）
 * - fin_payment_rec_detail: 支付明细表（插入支付明细）
 * - fin_payment_rec_static: 支付分类统计表（插入统计记录）
 * - adm_charge_item: 收费项目表（核销收费项）
 * - adm_invoice: 发票表（开具发票）
 * - adm_encounter: 就诊记录表（状态翻转）
 * - adm_account: 账户表（余额扣减）
 */
class ChargeConfirmDAO : public BaseDAO {
public:
    // --------------------- 更新操作 ---------------------
    /**
     * 更新支付草稿状态：DRAFT → COMPLETED
     * 表：fin_payment_reconciliation
     * 字段：status_enum, payment_enum, outcome_enum, bill_date, version
     *
     * @param paymentDraftId 支付草稿ID（const引用）
     * @param paymentEnum 支付方式（const引用，如"1"表示现金）
     * @param paymentNo 支付编号（const引用）
     * @param billDate 结算日期（const引用，如"2026-07-19"）
     * @param version 乐观锁版本号（防止并发冲突）
     * @return 影响行数（成功返回1，失败返回0）
     */
    int updatePaymentReconciliation(const std::string& paymentDraftId, const std::string& paymentEnum, const std::string& paymentNo,const std::string& billDate, int version);

    /**
     * 插入支付明细记录
     * 表：fin_payment_rec_detail
     * 字段：id, reconciliation_id, account_id, pay_enum, amount, charge_amount, pay_trans_date, result_enum, target_enum, version
     *
     * @param detailId 明细记录ID（const引用）
     * @param reconciliationId 对账ID（const引用）
     * @param accountId 账户ID（const引用）
     * @param payEnum 支付方式（const引用）
     * @param amount 支付金额
     * @param chargeAmount 实际收费金额
     * @param payTransDate 交易时间（const引用）
     * @param resultEnum 支付结果（const引用，如"SUCCESS"）
     * @param targetEnum 支付对象（const引用，如"PATIENT"）
     * @return 影响行数
     */
    int insertPaymentDetail(const std::string& detailId, const std::string& reconciliationId, std::string & accountId,
        const std::string& payEnum, double amount, double chargeAmount, const std::string& payTransDate,
        const std::string& resultEnum, const std::string& targetEnum);

    /**
     * 插入支付分类统计记录
     * 表：fin_payment_rec_static
     * 字段：reconciliation_id, method_code, type_code, type_name, amount, charge_item_ids, version
     *
     * @param reconciliationId 对账ID（const引用）
     * @param methodCode 支付方式编码（const引用，如"1"表示现金）
     * @param typeCode 费用类型编码（const引用，如"REG_FEE"表示挂号费）
     * @param typeName 费用类型名称（const引用，如"挂号费"）
     * @param amount 金额
     * @param chargeItemIds 关联收费项ID（const引用，如"123,456"）
     * @return 影响行数
     */
    int insertPaymentStatic(const std::string& reconciliationId, const std::string& methodCode, const std::string& typeCode,
        const std::string& typeName, double amount, const std::string& chargeItemIds);

    /**
     * 更新收费项状态：UNPAID → PAID
     * 表：adm_charge_item
     * 字段：status_enum, version
     *
     * @param paymentDraftId 收费项ID（const引用）
     * @return 影响行数
     */
    int updateChargeItemToPaid(const std::string& paymentDraftId);

    /**
     * 插入发票记录
     * 表：adm_invoice
     * 字段：id, patient_id, reconciliation_id, payment_id, bus_no, bill_no, type_code, amount, charge_item_ids, issued_at, payment_terms, version
     *
     * @param invoiceId 发票ID（const引用）
     * @param patientId 患者ID（const引用）
     * @param reconciliationId 对账ID（const引用）
     * @param paymentId 支付ID（const引用）
     * @param busNo 发票流水号（const引用）
     * @param billNo 发票号码（const引用）
     * @param typeCode 发票类型（const引用，如"OUTPATIENT"）
     * @param amount 发票金额
     * @param chargeItemIds 关联收费项ID（const引用）
     * @param issuedAt 开具时间（const引用）
     * @param paymentTerms 支付方式（const引用，如"CASH"）
     * @return 影响行数
     */
    int insertInvoice(const std::string& invoiceId, const std::string& patientId, const std::string& reconciliationId,
        const std::string& paymentId, const std::string& busNo, const std::string& billNo, const std::string& typeCode,
        double amount, const std::string& chargeItemIds, const std::string& issuedAt, const std::string& paymentTerms);

    /**
     * 更新就诊状态：PENDING → REGISTERED
     * 表：adm_encounter
     * 字段：status_enum, version
     *
     * @param encounterId 就诊ID（const引用）
     * @param version 乐观锁版本号
     * @return 影响行数
     */
    int updateEncounterToRegistered(const std::string& encounterId, int version);

    /**
     * 更新账户余额（扣减）
     * 表：adm_account
     * 字段：balance, version
     *
     * @param accountId 账户ID（const引用）
     * @param amount 扣减金额
     * @param version 乐观锁版本号
     * @return 影响行数
     */
    int updateAccountBalance(const std::string& accountId, double amount, int version);
};

#endif // !_CHARGE_CONFIRM_DAO_
