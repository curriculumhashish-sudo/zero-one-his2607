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

// 预交金流水链接用的账户快照
struct AccountSnapshot
{
    std::string accountId;      // 最新记录的account_id
    std::string predecessorId;  // 最新记录ID（用于流水链接）
    double balance;             // 最新余额
};

class PrepaymentDAO : public BaseDAO
{
private:
    inline std::string queryConditionBuilder(
        const PrepaymentQuery::Wrapper& query, SqlParams& params);

public:
    // 统计预交金记录数（分页用）
    uint64_t count(const PrepaymentQuery::Wrapper& query);

    // 分页查询预交金记录
    std::list<PrepaymentDO> selectWithPage(const PrepaymentQuery::Wrapper& query);

    // 获取最新账户快照用于余额链接
    AccountSnapshot getAccountSnapshot(const std::string& accountCode);

    // 插入预交金记录（收或退）
    void insertPrepayment(
        const std::string& id, const std::string& nowStr,
        const std::string& tenantId, const std::string& operatorName,
        const std::string& accountId, const std::string& accountCode,
        const std::string& payEnum, double amount,
        double chargeAmount, double returnAmount,
        const std::string& payTransNo, const std::string& payTransText,
        const std::string& payTransDate,
        double beforeBalance, double afterBalance,
        const std::string& predecessorId);
};

#endif // !_PREPAYMENT_DAO_
