#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: fanpai
 @Date: 2026/07/20 16:00:00

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
#ifndef _MEDICAL_ORDER_EXECUTION_DAO_H_
#define _MEDICAL_ORDER_EXECUTION_DAO_H_

#include "BaseDAO.h"
#include "domain/do/medicalorderexecution/WkfServiceRequestDO.h"
#include "domain/query/medicalorderexecution/MedicalOrderQuery.h"

/**
 * DAO for medical order execution and verification.
 * Operates on the wkf_service_request table.
 */
class MedicalOrderExecutionDAO : public BaseDAO
{
public:
    MedicalOrderExecutionDAO() = default;
    ~MedicalOrderExecutionDAO() = default;

    /**
     * Query orders pending verification (requirement 1: get verification items).
     * @param query filter conditions + pagination
     * @return list of orders awaiting verification
     */
    std::list<PtrWkfServiceRequestDO> queryVerificationItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Count verification items matching filter.
     * @param query filter conditions
     * @return total count
     */
    uint64_t countVerificationItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Query orders ready for execution (requirement 4: get execution items).
     * @param query filter conditions + pagination
     * @return list of orders ready for execution
     */
    std::list<PtrWkfServiceRequestDO> queryExecutionItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Count execution items matching filter.
     * @param query filter conditions
     * @return total count
     */
    uint64_t countExecutionItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Verify pass (requirement 2): update status to 'verified', record verifier and check time.
     * @param serviceRequestId order ID
     * @param verifierId nurse ID
     * @return rows affected (1=success, 0=not found/already processed)
     */
    int verifyPass(const std::string& serviceRequestId, const std::string& verifierId);

    /**
     * Return order (requirement 3): update status to 'returned', record rejection reason.
     * @param serviceRequestId order ID
     * @param returnReason explanation for rejection
     * @return rows affected (1=success, 0=not found/already processed)
     */
    int returnOrder(const std::string& serviceRequestId, const std::string& returnReason);
};

#endif // !_MEDICAL_ORDER_EXECUTION_DAO_H_
