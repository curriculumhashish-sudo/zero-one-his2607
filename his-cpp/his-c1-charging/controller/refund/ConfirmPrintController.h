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
#ifndef _CONFIRM_PRINT_CONTROLLER_
#define _CONFIRM_PRINT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/refund/ConfirmPrintQuery.h"
#include "domain/dto/refund/ConfirmPrintDTO.h"
#include "domain/vo/refund/ConfirmPrintVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("xbzml.charging.refund.tags.t1")

class ConfirmPrintController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(ConfirmPrintController);

public:
    // GET /charging/refund/confirm-print
    // Confirms that the refund receipt was printed successfully
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("xbzml.charging.refund.confirm-print.summary"),
        queryAll,
        ConfirmPrintQuery,
        ConfirmPrintJsonVO::Wrapper,
        API_TAG);

    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/refund/confirm-print",
        queryAll,
        ConfirmPrintQuery,
        execQueryAll(query));

private:
    ConfirmPrintJsonVO::Wrapper execQueryAll(
        const ConfirmPrintQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_CONFIRM_PRINT_CONTROLLER_
