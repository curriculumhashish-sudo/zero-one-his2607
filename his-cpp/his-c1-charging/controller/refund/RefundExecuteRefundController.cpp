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
#include "RefundExecuteRefundController.h"
#include "../../service/refund/RefundExecuteRefundService.h"

// Handle POST /charging/refund/execute-refund
// Executes refund as a transactional operation (INSERT + UPDATE)
RefundExecuteRefundJsonVO::Wrapper RefundExecuteRefundController::execRefundExecuteRefund(const RefundExecuteRefundRequestDTO::Wrapper& dto)
{
    auto vo = RefundExecuteRefundJsonVO::createShared();
    RefundExecuteRefundService service;
    auto result = service.execute(dto);
    vo->success(result);
    return vo;
}
