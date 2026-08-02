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

#ifndef _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_SERVICE_H_
#define _PAY_MANAGE_GET_PATIENT_PAY_RECORDS_SERVICE_H_

#include "domain/vo/pay_manage/Pay_ManageGetPatientPayRecordsVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientPayRecordsQuery.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientPayRecordsDTO.h"

/**
 * Patient prepayment records query service (Interface 3)
 *
 * Query flow: DAO query data -> DO to DTO conversion -> assemble paginated result
 */
class Pay_ManageGetPatientPayRecordsService
{
private:
    // Translate payment method code to display text
    static std::string translatePayType(const std::string& code);

    // Translate business type code to display text
    static std::string translateServiceType(const std::string& code);

    // Translate status code to display text
    static std::string translateStatus(const std::string& code);

public:
    /**
     * Paginated query for patient prepayment records
     * @param query frontend query parameters containing hospitalized_number,
     *              pageIndex, pageSize
     * @return paginated result containing content data list, total record count, pages count etc.
     */
    Pay_ManageGetPatientPayRecordsPageDTO::Wrapper listAll(const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query);
};

#endif // !_PAY_MANAGE_GET_PATIENT_PAY_RECORDS_SERVICE_H_
