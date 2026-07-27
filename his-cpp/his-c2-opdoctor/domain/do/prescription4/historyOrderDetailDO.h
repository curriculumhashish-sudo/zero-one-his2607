#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/27

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
#ifndef _HISTORY_ORDER_DETAIL_DO_
#define _HISTORY_ORDER_DETAIL_DO_
#include "../DoInclude.h"

class HistoryOrderDetailDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, orderId, OrderId);
    MYSQL_SYNTHESIZE(string, orderType, OrderType);
    MYSQL_SYNTHESIZE(string, orderName, OrderName);
    MYSQL_SYNTHESIZE(int, quantity, Quantity);
    MYSQL_SYNTHESIZE(int, executedCount, ExecutedCount);
    MYSQL_SYNTHESIZE(string, sampleType, SampleType);
    MYSQL_SYNTHESIZE(string, executeHospital, ExecuteHospital);
    MYSQL_SYNTHESIZE(string, createTime, CreateTime);
    MYSQL_SYNTHESIZE(string, diagnosis, Diagnosis);
    MYSQL_SYNTHESIZE(string, doctorName, DoctorName);
    MYSQL_SYNTHESIZE(int, isSkinTest, IsSkinTest);

public:
    HistoryOrderDetailDO() : BaseDO("med_medication_request")
    {
        MYSQL_ADD_FIELD_PK("order_id", "s", orderId);
        MYSQL_ADD_FIELD("order_type", "s", orderType);
        MYSQL_ADD_FIELD("order_name", "s", orderName);
        MYSQL_ADD_FIELD("quantity", "i", quantity);
        MYSQL_ADD_FIELD("executed_count", "i", executedCount);
        MYSQL_ADD_FIELD_NULLABLE("sample_type", "s", sampleType, false);
        MYSQL_ADD_FIELD("execute_hospital", "s", executeHospital);
        MYSQL_ADD_FIELD("create_time", "s", createTime);
        MYSQL_ADD_FIELD("diagnosis", "s", diagnosis);
        MYSQL_ADD_FIELD("doctor_name", "s", doctorName);
        MYSQL_ADD_FIELD_NULLABLE("is_skin_test", "i", isSkinTest, false);
    }
};

typedef std::shared_ptr<HistoryOrderDetailDO> PtrHistoryOrderDetailDO;

#endif // !_HISTORY_ORDER_DETAIL_DO_
