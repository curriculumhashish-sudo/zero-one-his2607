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

#ifndef _PAY_MANAGE_GET_PATIENT_QUEUE_DAO_H_
#define _PAY_MANAGE_GET_PATIENT_QUEUE_DAO_H_

#include "BaseDAO.h"
#include "domain/do/pay_manage/Pay_ManageGetPatientQueueDO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientQueueQuery.h"

/**
 * Patient queue query DAO
 * Provides count and paginated query for inpatient patient queue
 */
class Pay_ManageGetPatientQueueDAO : public BaseDAO
{
private:
    /**
     * Dynamically build WHERE clause based on query parameters
     * @param query frontend query parameters
     * @param params output parameters for prepared statement binding
     * @return WHERE clause string starting with " WHERE 1=1 ..."
     */
    inline std::string queryConditionBuilder(
        const Pay_ManageGetPatientQueueQuery::Wrapper& query, SqlParams& params);

public:
    /**
     * Count total records matching query conditions
     * @param query frontend query parameters
     * @return total count of matching inpatient encounters
     */
    uint64_t count(const Pay_ManageGetPatientQueueQuery::Wrapper& query);

    /**
     * Paginated query for patient queue
     * @param query frontend query parameters (including pageIndex and pageSize)
     * @return list of DOs for the current page
     */
    std::list<Pay_ManageGetPatientQueueDO> selectWithPage(const Pay_ManageGetPatientQueueQuery::Wrapper& query);
};

#endif // !_PAY_MANAGE_GET_PATIENT_QUEUE_DAO_H_
