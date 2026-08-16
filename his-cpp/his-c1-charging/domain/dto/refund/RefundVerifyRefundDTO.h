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
#ifndef _REFUND_VERIFY_REFUND_DTO_
#define _REFUND_VERIFY_REFUND_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 确认退费响应 DTO — 字段名与 sample-json 严格一一对应
// sample-json: {"allowed":true, "rejectReasonCode":null}
class RefundVerifyRefundDTO : public oatpp::DTO
{
    DTO_INIT(RefundVerifyRefundDTO, DTO);

    // 是否允许退费 — 对应 sample-json: "allowed"
    DTO_FIELD(Boolean, allowed);
    DTO_FIELD_INFO(allowed) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.allowed");
    };
    // 拒绝原因编码 — 对应 sample-json: "rejectReasonCode"（可退时为 null）
    // 注: 显式指定 QUALIFIER 与 NAME 相同，强制走 DTO_FIELD_2 宏展开路径
    DTO_FIELD(String, rejectReasonCode, "rejectReasonCode");
    DTO_FIELD_INFO(rejectReasonCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.rejectReasonCode");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_REFUND_VERIFY_REFUND_DTO_
