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
#include "RefundConfirmPrintService.h"
#include "../../dao/refund/RefundConfirmPrintDAO.h"

// Prevent macro conflicts
#include "NacosClient.h"

// Confirm print job status — called by electron-hiprint after printing
RefundConfirmPrintDTO::Wrapper RefundConfirmPrintService::confirm(const RefundConfirmPrintQuery::Wrapper& query)
{
    auto dto = RefundConfirmPrintDTO::createShared();

    std::string traceId = query->traceId.getValue("");
    if (traceId.empty()) {
        dto->success = false;
        return dto;
    }

    RefundConfirmPrintDAO dao;
    auto session = dao.getSqlSession();

    // UPDATE prt_print_event_log with print result
    // biz_ref_json is JSON type — use '{}' as fallback for empty input
    std::string bizRef = query->bizRef.getValue("");
    if (bizRef.empty()) bizRef = "{}";
    // 直接在Service组装sql，不调用dao更快
    std::ostringstream sql;
    sql << "UPDATE prt_print_event_log SET"
        << " status = ?,"
        << " copies = ?,"
        << " client_type = ?,"
        << " biz_ref_json = ?,"
        << " updated_at = NOW()"
        << " WHERE trace_id = ?";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, query->status.getValue(""));
    SQLPARAMS_PUSH(params, "i", int, (int)query->copies.getValue(1));
    SQLPARAMS_PUSH(params, "s", std::string, query->clientType.getValue(""));
    SQLPARAMS_PUSH(params, "s", std::string, bizRef);
    SQLPARAMS_PUSH(params, "s", std::string, traceId);

    int rows = session->executeUpdate(sql.str(), params);
    dto->success = (rows > 0);
    return dto;
}
