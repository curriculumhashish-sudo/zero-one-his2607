/*
 Copyright Zero One Star. All rights reserved.

 @Author: Jack
 @Date: 2026/07/20

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
#include "PrepaymentController.h"
#include "../../service/pay_manage/PrepaymentService.h"

// Add prepayment (charge)
PrepaymentAddVO::Wrapper PrepaymentController::execAddPrepayment(const PrepaymentAddDTO::Wrapper& dto)
{
    OATPP_LOGD("Prepayment", "execAddPrepayment called");
    auto vo = PrepaymentAddVO::createShared();
    PrepaymentService service;
    auto result = service.addPrepayment(dto);
    vo->success(result);
    OATPP_LOGD("Prepayment", "execAddPrepayment done");
    return vo;
}

// Refund prepayment
PrepaymentRefundVO::Wrapper PrepaymentController::execRefundPrepayment(const PrepaymentRefundDTO::Wrapper& dto)
{
    auto vo = PrepaymentRefundVO::createShared();
    PrepaymentService service;
    auto result = service.refundPrepayment(dto);
    vo->success(result);
    return vo;
}

// Query prepayment records
PrepaymentPageJsonVO::Wrapper PrepaymentController::execQueryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    auto vo = PrepaymentPageJsonVO::createShared();
    PrepaymentService service;
    auto result = service.queryPrepayment(query);
    vo->success(result);
    return vo;
}
