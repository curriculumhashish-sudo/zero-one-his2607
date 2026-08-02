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

#ifndef _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_MAPPER_H_
#define _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_MAPPER_H_

#include "Mapper.h"
#include "domain/do/pay_manage/Pay_ManageGetPatientPayRecordsDO.h"

/**
 * Result set mapper for Pay_ManageGetPatientPayRecordsDO
 */
class Pay_ManageGetPatientPayRecordsMapper : public Mapper<Pay_ManageGetPatientPayRecordsDO>
{
public:
    Pay_ManageGetPatientPayRecordsDO mapper(ResultSet* resultSet) const override
    {
        Pay_ManageGetPatientPayRecordsDO data;

        data.setPayTime(resultSet->getString("pay_time"));
        data.setServiceType(resultSet->getString("service_type"));
        data.setCost(resultSet->getString("cost"));
        data.setPayType(resultSet->getString("pay_type"));
        data.setOperateman(resultSet->getString("operateman"));
        data.setBillNumber(resultSet->getString("bill_number"));
        data.setStatus(resultSet->getString("status"));

        return data;
    }
};

#endif // !_PAY_MANAGE_GET_PATIENT_PAY_RECORDS_MAPPER_H_
