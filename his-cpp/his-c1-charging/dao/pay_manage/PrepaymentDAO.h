#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Jack
 @Date: 2026/07/22

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

#ifndef _PREPAYMENT_DAO_
#define _PREPAYMENT_DAO_

#include "BaseDAO.h"
#include "domain/do/pay_manage/PrepaymentDO.h"
#include "domain/query/pay_manage/PrepaymentQuery.h"

class PrepaymentDAO : public BaseDAO
{
private:
    // Build WHERE clause dynamically based on query conditions
    inline std::string queryConditionBuilder(
        const PrepaymentQuery::Wrapper& query, SqlParams& params);

public:
    // Count prepayment records (for pagination)
    uint64_t count(const PrepaymentQuery::Wrapper& query);

    // Page query prepayment records
    std::list<PrepaymentDO> selectWithPage(const PrepaymentQuery::Wrapper& query);

    // Insert a new prepayment record (returns auto-generated PK)
    uint64_t addPrepayment(const PrepaymentDO& prepayment);

    // Get the latest after_balance for an account (used as before_balance for new tx)
    double getLatestBalance(const std::string& accountCode);

    // Get the account_id by account_code (inpatientNo)
    std::string getAccountIdByCode(const std::string& accountCode);
};

#endif // !_PREPAYMENT_DAO_
