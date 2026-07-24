#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

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
#ifndef _REFUND_EXECUTE_REFUND_DO_
#define _REFUND_EXECUTE_REFUND_DO_

#include "../DoInclude.h"

// 执行退费数据对象
// 注: 本接口为事务性写操作，DO 用于承载执行结果
class RefundExecuteRefundDO : public BaseDO
{
    // 是否执行成功
    MYSQL_SYNTHESIZE(string, success, Success);
    // 冲正支付ID
    MYSQL_SYNTHESIZE(string, reversePaymentId, ReversePaymentId);
    // 原始支付ID
    MYSQL_SYNTHESIZE(string, originalPaymentId, OriginalPaymentId);
    // 是否半退
    MYSQL_SYNTHESIZE(string, halfRefundExecuted, HalfRefundExecuted);
    // 医保是否已撤销
    MYSQL_SYNTHESIZE(string, insuranceRevoked, InsuranceRevoked);

public:
    RefundExecuteRefundDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("success", "s", success);
        MYSQL_ADD_FIELD("reverse_payment_id", "s", reversePaymentId);
        MYSQL_ADD_FIELD("original_payment_id", "s", originalPaymentId);
        MYSQL_ADD_FIELD("half_refund_executed", "s", halfRefundExecuted);
        MYSQL_ADD_FIELD("insurance_revoked", "s", insuranceRevoked);
    }
};

typedef std::shared_ptr<RefundExecuteRefundDO> PtrRefundExecuteRefundDO;

#endif // !_REFUND_EXECUTE_REFUND_DO_
