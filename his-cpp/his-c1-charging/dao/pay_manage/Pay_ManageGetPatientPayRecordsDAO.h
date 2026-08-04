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

#ifndef _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_DAO_H_
#define _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_DAO_H_

#include "BaseDAO.h"
#include "domain/do/pay_manage/Pay_ManageGetPatientPayRecordsDO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientPayRecordsQuery.h"

/**
 * Patient prepayment records query DAO
 * Provides count and paginated query for payment reconciliation records
 */
class Pay_ManageGetPatientPayRecordsDAO : public BaseDAO
{
public:
    /**
     * Count total payment records for the given patient
     * @param query frontend query parameters containing hospitalized_number
     * @return total count of payment records
     */
    uint64_t count(const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query);

    /**
     * Paginated query for payment records
     * @param query frontend query parameters (including pageIndex and pageSize)
     * @return list of DOs for the current page
     */
    std::list<Pay_ManageGetPatientPayRecordsDO> selectWithPage(const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query);
};

#endif // !_PAY_MANAGE_GET_PATIENT_PAY_RECORDS_DAO_H_
