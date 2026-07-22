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
#ifndef _REFUND_REFUND_EXECUTE_REFUND_CONTROLLER_
#define _REFUND_REFUND_EXECUTE_REFUND_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/refund/RefundExecuteRefundDTO.h"
#include "domain/vo/refund/RefundExecuteRefundVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.refund.tag")

class RefundExecuteRefundController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(RefundExecuteRefundController);

public:
    // POST /charging/refund/execute-refund
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("xbzml.charging.refund.execute-refund.summary"),
        executeRefund,
        RefundExecuteRefundJsonVO::Wrapper,
        API_TAG);

    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/charging/refund/execute-refund",
        executeRefund,
        BODY_DTO(RefundExecuteRefundRequestDTO::Wrapper, dto),
        execRefundExecuteRefund(dto));

private:
    RefundExecuteRefundJsonVO::Wrapper execRefundExecuteRefund(
        const RefundExecuteRefundRequestDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_REFUND_REFUND_EXECUTE_REFUND_CONTROLLER_
