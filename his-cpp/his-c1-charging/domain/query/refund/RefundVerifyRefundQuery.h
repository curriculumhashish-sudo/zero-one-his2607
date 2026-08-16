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
#ifndef _REFUND_VERIFY_REFUND_QUERY_
#define _REFUND_VERIFY_REFUND_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 确认退费 — 查询参数
// GET /charging/refund/verify-refund?encounterId=xxx&refundingChargeItemIds=id1,id2
class RefundVerifyRefundQuery : public PageQuery
{
    DTO_INIT(RefundVerifyRefundQuery, PageQuery);

    // 就诊ID（必填）
    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterId");
    };

    // 待退费收费项目ID列表（逗号分隔）
    // 注: 显式指定 QUALIFIER 与 NAME 相同，强制走 DTO_FIELD_2 宏展开路径
    DTO_FIELD(String, refundingChargeItemIds, "refundingChargeItemIds");
    DTO_FIELD_INFO(refundingChargeItemIds) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.refundingChargeItemIds");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_REFUND_VERIFY_REFUND_QUERY_
