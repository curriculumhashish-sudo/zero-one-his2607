#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Jack
 @Date: 2026/07/26 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _PREPAYMENT_DO_
#define _PREPAYMENT_DO_

#include "../DoInclude.h"

// 预交金收支记录DO，对应 fin_payment_rec_detail 表
// 收预交金：pay_enum=CHARGE, charge_amount=金额, amount=金额
// 退预交金：pay_enum=REFUND, return_amount=金额, amount=金额
// 通过 target_enum 区分业务类型，如 PREPAYMENT
class PrepaymentDO : public BaseDO
{
    // ========== 系统字段 ==========
    // 主键
    MYSQL_SYNTHESIZE(string, id, Id);
    // 租户标识
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    // 创建时间
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    // 更新时间
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    // 创建人
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    // 更新人
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    // 逻辑删除标记（0-未删除, 1-已删除）
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    // 删除时间
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    // 删除人
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    // 乐观锁版本号
    MYSQL_SYNTHESIZE(int, version, Version);

    // ========== 业务字段 ==========
    // 对账标识（关联 fin_payment_reconciliation）
    MYSQL_SYNTHESIZE(string, reconciliationId, ReconciliationId);
    // 上一笔记录ID（流水链接）
    MYSQL_SYNTHESIZE(string, predecessorId, PredecessorId);
    // 业务目标类型，如 PREPAYMENT 表示预交金
    MYSQL_SYNTHESIZE(string, targetEnum, TargetEnum);
    // 账户ID（关联患者账户）
    MYSQL_SYNTHESIZE(string, accountId, AccountId);
    // 账户编码
    MYSQL_SYNTHESIZE(string, accountCode, AccountCode);
    // 支付类型：CHARGE-收, REFUND-退
    MYSQL_SYNTHESIZE(string, payEnum, PayEnum);
    // 支付层级
    MYSQL_SYNTHESIZE(string, payLevelEnum, PayLevelEnum);
    // 交易金额
    MYSQL_SYNTHESIZE(double, amount, Amount);
    // 退款金额（退预交金时使用）
    MYSQL_SYNTHESIZE(double, returnAmount, ReturnAmount);
    // 收费金额（收预交金时使用）
    MYSQL_SYNTHESIZE(double, chargeAmount, ChargeAmount);
    // 支付流水号
    MYSQL_SYNTHESIZE(string, payTransNo, PayTransNo);
    // 支付说明文本
    MYSQL_SYNTHESIZE(string, payTransText, PayTransText);
    // 支付时间
    MYSQL_SYNTHESIZE(string, payTransDate, PayTransDate);
    // 交易前余额
    MYSQL_SYNTHESIZE(double, beforeBalance, BeforeBalance);
    // 交易后余额
    MYSQL_SYNTHESIZE(double, afterBalance, AfterBalance);
    // 支付结果
    MYSQL_SYNTHESIZE(string, resultEnum, ResultEnum);

public:
    PrepaymentDO() : BaseDO("fin_payment_rec_detail")
    {
        // ========== 系统字段映射 ==========
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("created_at", "s", createdAt);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
        MYSQL_ADD_FIELD("created_by", "s", createdBy);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
        MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
        MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
        MYSQL_ADD_FIELD("version", "i", version);

        // ========== 业务字段映射 ==========
        MYSQL_ADD_FIELD("reconciliation_id", "s", reconciliationId);
        MYSQL_ADD_FIELD("predecessor_id", "s", predecessorId);
        MYSQL_ADD_FIELD("target_enum", "s", targetEnum);
        MYSQL_ADD_FIELD("account_id", "s", accountId);
        MYSQL_ADD_FIELD("account_code", "s", accountCode);
        MYSQL_ADD_FIELD("pay_enum", "s", payEnum);
        MYSQL_ADD_FIELD("pay_level_enum", "s", payLevelEnum);
        MYSQL_ADD_FIELD("amount", "d", amount);
        MYSQL_ADD_FIELD("return_amount", "d", returnAmount);
        MYSQL_ADD_FIELD("charge_amount", "d", chargeAmount);
        MYSQL_ADD_FIELD("pay_trans_no", "s", payTransNo);
        MYSQL_ADD_FIELD("pay_trans_text", "s", payTransText);
        MYSQL_ADD_FIELD("pay_trans_date", "s", payTransDate);
        MYSQL_ADD_FIELD("before_balance", "d", beforeBalance);
        MYSQL_ADD_FIELD("after_balance", "d", afterBalance);
        MYSQL_ADD_FIELD("result_enum", "s", resultEnum);
    }
};

typedef std::shared_ptr<PrepaymentDO> PtrPrepaymentDO;

#endif // !_PREPAYMENT_DO_
