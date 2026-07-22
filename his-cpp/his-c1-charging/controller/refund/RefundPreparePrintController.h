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
#ifndef _REFUND_REFUND_PREPARE_PRINT_CONTROLLER_
#define _REFUND_REFUND_PREPARE_PRINT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/refund/RefundPreparePrintQuery.h"
#include "domain/dto/refund/RefundPreparePrintDTO.h"
#include "domain/vo/refund/RefundPreparePrintVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.refund.tag")

class RefundPreparePrintController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(RefundPreparePrintController);

public:
    // GET /charging/refund/prepare-print
    // Returns the PrintJson template for refund receipt printing
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("xbzml.charging.refund.prepare-print.summary"),
        queryAll,
        RefundPreparePrintQuery,
        RefundPreparePrintJsonVO::Wrapper,
        API_TAG);

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/refund/prepare-print",
        queryAll,
        RefundPreparePrintQuery,
        execQueryAll(query));

private:
    RefundPreparePrintJsonVO::Wrapper execQueryAll(
        const RefundPreparePrintQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_REFUND_REFUND_PREPARE_PRINT_CONTROLLER_
