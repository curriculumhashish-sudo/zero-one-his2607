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
#ifndef _REFUND_PREPARE_PRINT_DO_
#define _REFUND_PREPARE_PRINT_DO_

#include "../DoInclude.h"

// RefundPreparePrint data object ¡ª print job record
class RefundPreparePrintDO : public BaseDO
{
    // Print job trace ID
    MYSQL_SYNTHESIZE(string, traceId, TraceId);
    // Business number (e.g., "RT202606260005")
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    // Print binding code
    MYSQL_SYNTHESIZE(string, bindingCode, BindingCode);

public:
    RefundPreparePrintDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("trace_id", "s", traceId);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("binding_code", "s", bindingCode);
    }
};

typedef std::shared_ptr<RefundPreparePrintDO> PtrRefundPreparePrintDO;

#endif // !_REFUND_PREPARE_PRINT_DO_
