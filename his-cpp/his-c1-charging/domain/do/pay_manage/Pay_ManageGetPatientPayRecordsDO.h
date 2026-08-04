#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: byj
 @Date: 2026/08/02

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

#ifndef _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_DO_H_
#define _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_DO_H_

#include "../DoInclude.h"

/**
 * Patient prepayment records data object
 * Corresponds to the result set of the payment reconciliation query
 * BaseDO("") indicates this is a read-only DO not bound to a single table
 */
class Pay_ManageGetPatientPayRecordsDO : public BaseDO
{
    // Charge time (payment bill_date)
    MYSQL_SYNTHESIZE(std::string, payTime, PayTime);
    // Business type (payment kind_enum)
    MYSQL_SYNTHESIZE(std::string, serviceType, ServiceType);
    // Amount (payment display_amount)
    MYSQL_SYNTHESIZE(std::string, cost, Cost);
    // Payment method (payment detail pay_enum)
    MYSQL_SYNTHESIZE(std::string, payType, PayType);
    // Operator name (whale_users name)
    MYSQL_SYNTHESIZE(std::string, operateman, Operateman);
    // Bill number (adm_invoice bill_no)
    MYSQL_SYNTHESIZE(std::string, billNumber, BillNumber);
    // Status (payment status_enum)
    MYSQL_SYNTHESIZE(std::string, status, Status);

public:
    Pay_ManageGetPatientPayRecordsDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("pay_time", "s", payTime);
        MYSQL_ADD_FIELD("service_type", "s", serviceType);
        MYSQL_ADD_FIELD("cost", "s", cost);
        MYSQL_ADD_FIELD("pay_type", "s", payType);
        MYSQL_ADD_FIELD("operateman", "s", operateman);
        MYSQL_ADD_FIELD("bill_number", "s", billNumber);
        MYSQL_ADD_FIELD("status", "s", status);
    }
};

#endif // !_PAY_MANAGE_GET_PATIENT_PAY_RECORDS_DO_H_
