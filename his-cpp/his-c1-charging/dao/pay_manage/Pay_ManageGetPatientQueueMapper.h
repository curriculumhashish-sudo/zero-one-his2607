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

#ifndef _PAY_MANAGE_GET_PATIENT_QUEUE_MAPPER_H_
#define _PAY_MANAGE_GET_PATIENT_QUEUE_MAPPER_H_

#include "Mapper.h"
#include "domain/do/pay_manage/Pay_ManageGetPatientQueueDO.h"

/**
 * Result set mapper for Pay_ManageGetPatientQueueDO
 * SQL column aliases must strictly match the getString() parameter names
 */
class Pay_ManageGetPatientQueueMapper : public Mapper<Pay_ManageGetPatientQueueDO>
{
public:
    Pay_ManageGetPatientQueueDO mapper(ResultSet* resultSet) const override
    {
        Pay_ManageGetPatientQueueDO data;

        data.setHospitalizedNumber(resultSet->getString("hospitalized_number"));
        data.setPatientName(resultSet->getString("patient_name"));
        data.setGenderCode(resultSet->getString("gender_code"));
        data.setBirthDate(resultSet->getString("birth_date"));
        data.setAdmissionTime(resultSet->getString("admission_time"));
        data.setDepartmentName(resultSet->getString("department_name"));
        data.setBedNumber(resultSet->getString("bed_number"));
        data.setCostNature(resultSet->getString("cost_nature"));
        data.setAccountBalance(resultSet->getString("account_balance"));

        return data;
    }
};

#endif // !_PAY_MANAGE_GET_PATIENT_QUEUE_MAPPER_H_
