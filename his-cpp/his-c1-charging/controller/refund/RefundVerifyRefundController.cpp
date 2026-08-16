/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
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
#include "stdafx.h"
#include "RefundVerifyRefundController.h"
#include "../../service/refund/RefundVerifyRefundService.h"

// Handle GET /charging/refund/verify-refund
// Validates whether selected charge items can be refunded
RefundVerifyRefundJsonVO::Wrapper RefundVerifyRefundController::execQueryAll(const RefundVerifyRefundQuery::Wrapper& query)
{
    auto vo = RefundVerifyRefundJsonVO::createShared();
    RefundVerifyRefundService service;
    auto dto = service.verify(query);
    vo->success(dto);
    return vo;
}
