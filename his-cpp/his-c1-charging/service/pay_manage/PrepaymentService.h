#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Jack
 @Date: 2026/07/26

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

#ifndef _PREPAYMENT_SERVICE_
#define _PREPAYMENT_SERVICE_

#include "domain/dto/pay_manage/PrepaymentDTO.h"
#include "domain/query/pay_manage/PrepaymentQuery.h"

class PrepaymentService
{
public:
    // Add prepayment (charge)
    PrepaymentAddResponseDTO::Wrapper addPrepayment(const PrepaymentAddDTO::Wrapper& dto);

    // Query prepayment records
    PrepaymentPageDTO::Wrapper queryPrepayment(const PrepaymentQuery::Wrapper& query);

    // Refund prepayment
    PrepaymentRefundResponseDTO::Wrapper refundPrepayment(const PrepaymentRefundDTO::Wrapper& dto);
};

#endif // !_PREPAYMENT_SERVICE_
