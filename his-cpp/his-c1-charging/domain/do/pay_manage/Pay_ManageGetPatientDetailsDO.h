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

#ifndef _PAY_MANAGE_GET_PATIENT_DETAILS_DO_H_
#define _PAY_MANAGE_GET_PATIENT_DETAILS_DO_H_

#include "../DoInclude.h"

/**
 * Patient details data object
 * Corresponds to the result set of the multi-table JOIN query for single patient
 * BaseDO("") indicates this is a read-only DO not bound to a single table
 */
class Pay_ManageGetPatientDetailsDO : public BaseDO
{
    // Hospitalized number (encounter bus_no)
    MYSQL_SYNTHESIZE(std::string, hospitalizedNumber, HospitalizedNumber);
    // Patient name
    MYSQL_SYNTHESIZE(std::string, patientName, PatientName);
    // Department name (from adm_organization)
    MYSQL_SYNTHESIZE(std::string, departmentName, DepartmentName);
    // Ward name (from adm_organization parent)
    MYSQL_SYNTHESIZE(std::string, wardName, WardName);
    // Bed number (from adm_location)
    MYSQL_SYNTHESIZE(std::string, bedNumber, BedNumber);
    // Account balance (from adm_account)
    MYSQL_SYNTHESIZE(std::string, accountBalance, AccountBalance);
    // Visit status (encounter status_enum)
    MYSQL_SYNTHESIZE(std::string, visitStatus, VisitStatus);
    // Cost nature name (from fin_contract)
    MYSQL_SYNTHESIZE(std::string, costNature, CostNature);

public:
    Pay_ManageGetPatientDetailsDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("hospitalized_number", "s", hospitalizedNumber);
        MYSQL_ADD_FIELD("patient_name", "s", patientName);
        MYSQL_ADD_FIELD("department_name", "s", departmentName);
        MYSQL_ADD_FIELD("ward_name", "s", wardName);
        MYSQL_ADD_FIELD("bed_number", "s", bedNumber);
        MYSQL_ADD_FIELD("account_balance", "s", accountBalance);
        MYSQL_ADD_FIELD("visit_status", "s", visitStatus);
        MYSQL_ADD_FIELD("cost_nature", "s", costNature);
    }
};

#endif // !_PAY_MANAGE_GET_PATIENT_DETAILS_DO_H_
