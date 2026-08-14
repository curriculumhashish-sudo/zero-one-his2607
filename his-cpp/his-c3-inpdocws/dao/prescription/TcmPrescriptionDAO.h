#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 Declares database operations used by TCM prescription actions.

 @Author: nick1484065161
 @Date: 2026/07/23 00:00:00

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

// Prevents duplicate DAO declarations when this header is included repeatedly.
#ifndef _TCM_PRESCRIPTION_DAO_H_
#define _TCM_PRESCRIPTION_DAO_H_

#include "stdafx.h"

#include "TcmPrescriptionMapper.h"
#include "BaseDAO.h"
#include "../../domain/do/prescription/MedMedicationRequestDO.h"

#include <list>

/**
 * Provides database access required by TCM prescription state changes.
 */
class TcmPrescriptionDAO : public BaseDAO
{
public:
    // Selects and locks one medication request by its primary key.
    PtrMedMedicationRequestDO selectByIdForUpdate(const std::string& id);

    // Selects and locks all medication requests under one prescription number.
    std::list<PtrMedMedicationRequestDO> selectByPrescriptionNoForUpdate(
        const std::string& prescriptionNo,
        const std::string& patientId,
        const std::string& encounterId
    );

    // Counts dispensing, charging, or execution records that block withdrawal.
    int countWithdrawalBlockingRecords(
        const std::string& requestId,
        const std::string& prescriptionNo,
        const std::string& tenantId
    );

    // Signs one draft medication request and assigns its prescription number.
    int updateForSign(
        const MedMedicationRequestDO& current,
        const std::string& prescriptionNo,
        const std::string& targetStatus,
        const std::string& changedTime,
        const std::string& operatorId,
        const std::string& expectedStatus
    );

    // Updates one medication request for prescription withdrawal.
    int updateForWithdraw(
        const MedMedicationRequestDO& current,
        const std::string& targetStatus,
        const std::string& changedTime,
        const std::string& operatorId,
        const std::string& expectedStatus
    );

    // Updates one medication request with a prescribed stop time.
    int updateForStop(
        const MedMedicationRequestDO& current,
        const std::string& targetStatus,
        const std::string& stopTime,
        const std::string& changedTime,
        const std::string& operatorId,
        const std::string& expectedStatus
    );

    // Cancels a previous stop and clears its effective dose end time.
    int updateForCancelStop(
        const MedMedicationRequestDO& current,
        const std::string& targetStatus,
        const std::string& changedTime,
        const std::string& operatorId,
        const std::string& expectedStatus
    );
};

#endif // !_TCM_PRESCRIPTION_DAO_H_
