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
#include "RefundPreparePrintController.h"
#include "../../service/refund/RefundPreparePrintService.h"

// Handle GET /charging/refund/prepare-print
// Returns print template for refund receipt
RefundPreparePrintJsonVO::Wrapper RefundPreparePrintController::execQueryAll(const RefundPreparePrintQuery::Wrapper& query)
{
    auto vo = RefundPreparePrintJsonVO::createShared();
    RefundPreparePrintService service;
    auto dto = service.prepare(query);
    vo->success(dto);
    return vo;
}
