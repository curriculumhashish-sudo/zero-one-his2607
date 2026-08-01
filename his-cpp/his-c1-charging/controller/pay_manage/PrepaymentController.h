#pragma once

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

#ifndef _PREPAYMENT_CONTROLLER_
#define _PREPAYMENT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/pay_manage/PrepaymentDTO.h"
#include "domain/vo/pay_manage/PrepaymentVO.h"
#include "domain/query/pay_manage/PrepaymentQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.tag")

/*
   Prepayment management controller
   Handles: add prepayment, refund prepayment, query records
*/
class PrepaymentController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PrepaymentController);

public:
    // Add prepayment (charge)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("Jack.charging.charge.prepayment.add.summary"),
        addPrepayment,
        PrepaymentAddVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/pay_manage/charge/add-prepayment", addPrepayment,
        BODY_DTO(PrepaymentAddDTO::Wrapper, dto), execAddPrepayment(dto)
    );

    // Refund prepayment
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("Jack.charging.charge.prepayment.refund.summary"),
        refundPrepayment,
        PrepaymentRefundVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/pay_manage/charge/refund-prepayment", refundPrepayment,
        BODY_DTO(PrepaymentRefundDTO::Wrapper, dto), execRefundPrepayment(dto)
    );

    // Query prepayment records
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("Jack.charging.charge.prepayment.query.summary"),
        queryPrepayment,
        PrepaymentPageJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_PAGE_PARAMS();
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "inpatientNo",
            ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentQuery.inpatientNo.description"),
            ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentQuery.inpatientNo.value"),
            true
        );
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/pay_manage/charge/query-prepayment", queryPrepayment,
        PrepaymentQuery, execQueryPrepayment(query)
    );

private:
    // Execute add prepayment
    PrepaymentAddVO::Wrapper execAddPrepayment(const PrepaymentAddDTO::Wrapper& dto);

    // Execute refund prepayment
    PrepaymentRefundVO::Wrapper execRefundPrepayment(const PrepaymentRefundDTO::Wrapper& dto);

    // Execute query prepayment records
    PrepaymentPageJsonVO::Wrapper execQueryPrepayment(const PrepaymentQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif // !_PREPAYMENT_CONTROLLER_
