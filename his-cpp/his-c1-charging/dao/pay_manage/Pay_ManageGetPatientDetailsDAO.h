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

#ifndef _PAY_MANAGE_GET_PATIENT_DETAILS_DAO_H_
#define _PAY_MANAGE_GET_PATIENT_DETAILS_DAO_H_

#include "BaseDAO.h"
#include "domain/do/pay_manage/Pay_ManageGetPatientDetailsDO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientDetailsQuery.h"

/**
 * Patient details query DAO
 * Provides single-record query by hospitalized number
 */
class Pay_ManageGetPatientDetailsDAO : public BaseDAO
{
public:
    /**
     * Query patient details by hospitalized number
     * @param query frontend query parameter containing hospitalized_number
     * @return patient details DO, or empty DO if not found
     */
    Pay_ManageGetPatientDetailsDO selectByBusNo(const Pay_ManageGetPatientDetailsQuery::Wrapper& query);
};

#endif // !_PAY_MANAGE_GET_PATIENT_DETAILS_DAO_H_
