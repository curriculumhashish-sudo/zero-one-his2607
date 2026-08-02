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

#ifndef _PAY_MANAGE_GET_PATIENT_QUEUE_SERVICE_H_
#define _PAY_MANAGE_GET_PATIENT_QUEUE_SERVICE_H_

#include "domain/vo/pay_manage/Pay_ManageGetPatientQueueVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientQueueQuery.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientQueueDTO.h"

/**
 * Patient queue query service (Interface 1)
 *
 * Query flow: DAO query data -> DO to DTO conversion -> compute supplementary fields in memory -> assemble paginated result
 */
class Pay_ManageGetPatientQueueService
{
private:
    // Translate gender code to display text (MALE -> Male, FEMALE -> Female)
    static std::string translateGender(const std::string& code);

    // Compute age display text from birth date, e.g. "1990-05-15" computes to "35"
    static std::string computeAge(const std::string& birthDate);

public:
    /**
     * Paginated query for patient queue
     * @param query frontend query conditions including hospitalized_number, name, bed_number,
     *              hospitalized_time, pageIndex, pageSize
     * @return paginated result containing content data list, total record count, pages count etc.
     */
    Pay_ManageGetPatientQueuePageDTO::Wrapper listAll(const Pay_ManageGetPatientQueueQuery::Wrapper& query);
};

#endif // !_PAY_MANAGE_GET_PATIENT_QUEUE_SERVICE_H_
