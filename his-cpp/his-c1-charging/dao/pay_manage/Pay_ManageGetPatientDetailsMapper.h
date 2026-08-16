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

#ifndef _PAY_MANAGE_GET_PATIENT_DETAILS_MAPPER_H_
#define _PAY_MANAGE_GET_PATIENT_DETAILS_MAPPER_H_

#include "Mapper.h"
#include "domain/do/pay_manage/Pay_ManageGetPatientDetailsDO.h"

/**
 * Result set mapper for Pay_ManageGetPatientDetailsDO
 */
class Pay_ManageGetPatientDetailsMapper : public Mapper<Pay_ManageGetPatientDetailsDO>
{
public:
    Pay_ManageGetPatientDetailsDO mapper(ResultSet* resultSet) const override
    {
        Pay_ManageGetPatientDetailsDO data;

        data.setHospitalizedNumber(resultSet->getString("hospitalized_number"));
        data.setPatientName(resultSet->getString("patient_name"));
        data.setDepartmentName(resultSet->getString("department_name"));
        data.setWardName(resultSet->getString("ward_name"));
        data.setBedNumber(resultSet->getString("bed_number"));
        data.setAccountBalance(resultSet->getString("account_balance"));
        data.setVisitStatus(resultSet->getString("visit_status"));
        data.setCostNature(resultSet->getString("cost_nature"));

        return data;
    }
};

#endif // !_PAY_MANAGE_GET_PATIENT_DETAILS_MAPPER_H_
