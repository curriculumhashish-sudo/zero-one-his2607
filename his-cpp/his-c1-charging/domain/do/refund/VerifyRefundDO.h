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
#ifndef _VERIFY_REFUND_DO_
#define _VERIFY_REFUND_DO_

#include "../DoInclude.h"

// 退费校验数据对象
// 注: 本接口为业务校验，不直接对应单表，DO 用于承载校验结果
class VerifyRefundDO : public BaseDO
{
    // 是否允许退费
    MYSQL_SYNTHESIZE(string, allowed, Allowed);
    // 拒绝原因编码
    MYSQL_SYNTHESIZE(string, rejectReasonCode, RejectReasonCode);

public:
    VerifyRefundDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("allowed", "s", allowed);
        MYSQL_ADD_FIELD("reject_reason_code", "s", rejectReasonCode);
    }
};

typedef std::shared_ptr<VerifyRefundDO> PtrVerifyRefundDO;

#endif // !_VERIFY_REFUND_DO_
