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
#ifndef _REFUND_CONFIRM_PRINT_MAPPER_
#define _REFUND_CONFIRM_PRINT_MAPPER_

#include "Mapper.h"
#include "../../domain/do/refund/RefundConfirmPrintDO.h"

class RefundConfirmPrintMapper : public Mapper<RefundConfirmPrintDO>
{
public:
    RefundConfirmPrintDO mapper(ResultSet* resultSet) const override
    {
        RefundConfirmPrintDO data;
        data.setTraceId(resultSet->getString("trace_id"));
        data.setBusNo(resultSet->getString("bus_no"));
        data.setScenarioCode(resultSet->getString("scenario_code"));
        data.setStatus(resultSet->getString("status"));
        data.setCopies(resultSet->getInt("copies"));
        data.setClientType(resultSet->getString("client_type"));
        return data;
    }
};

#endif // !_REFUND_CONFIRM_PRINT_MAPPER_
