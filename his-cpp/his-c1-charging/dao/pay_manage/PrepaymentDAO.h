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

// Account snapshot for balance chain linking
struct AccountSnapshot
{
    std::string accountId;      // Latest account_id
    std::string predecessorId;  // Latest record ID for balance chain
    double balance;             // Latest balance
};

// Resolved encounter info from inpatient number
struct EncounterInfo
{
    std::string encounterId;    // adm_encounter.id
    std::string patientId;      // adm_encounter.patient_id
    std::string accountId;      // adm_account.id
    std::string accountCode;    // adm_account.bus_no
    double balance;             // adm_account.balance
};

class PrepaymentDAO : public BaseDAO
{
private:
    inline std::string queryConditionBuilder(
        const PrepaymentQuery::Wrapper& query, SqlParams& params);

public:
    // Count prepayment records (for pagination)
    uint64_t count(const PrepaymentQuery::Wrapper& query);

    // Paginated query for prepayment records
    std::list<PrepaymentDO> selectWithPage(const PrepaymentQuery::Wrapper& query);

    // Get latest account snapshot for balance chain
    AccountSnapshot getAccountSnapshot(const std::string& accountCode);

    // Resolve encounter info from inpatient number (adm_encounter JOIN adm_account)
    EncounterInfo resolveEncounter(const std::string& inpatientNo);

    // Insert payment reconciliation main record, returns reconciliation_id
    std::string insertReconciliation(
        const std::string& id, const std::string& nowStr,
        const std::string& tenantId, const std::string& operatorId,
        const std::string& paymentNo, const std::string& paymentEnum,
        const std::string& patientId, const std::string& encounterId,
        const std::string& accountId,
        double tenderedAmount, double returnedAmount,
        const std::string& billDate);

    // Update account balance in adm_account
    void updateAccountBalance(const std::string& accountId, double newBalance,
        const std::string& nowStr);

    // Insert prepayment detail record into fin_payment_rec_detail
    void insertPrepayment(
        const std::string& id, const std::string& nowStr,
        const std::string& tenantId, const std::string& operatorName,
        const std::string& reconciliationId,
        const std::string& accountId, const std::string& accountCode,
        const std::string& payEnum, double amount,
        double chargeAmount, double returnAmount,
        const std::string& payTransNo, const std::string& payTransText,
        const std::string& payTransDate,
        double beforeBalance, double afterBalance,
        const std::string& predecessorId);
};

#endif // !_PREPAYMENT_DAO_
