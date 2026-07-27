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
#include "RefundGetItemController.h"
#include "../../service/refund/RefundGetItemService.h"

// Handle GET /charging/refund/get-item-list
// Delegates to Service layer and wraps result in JsonVO response envelope
RefundGetItemJsonVO::Wrapper RefundGetItemController::execQueryAll(const RefundGetItemQuery::Wrapper& query)
{
    auto vo = RefundGetItemJsonVO::createShared();
    RefundGetItemService service;
    auto dto = service.listAll(query);
    vo->success(dto);
    return vo;
}
