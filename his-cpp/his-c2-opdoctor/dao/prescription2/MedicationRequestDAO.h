#pragma once

#ifndef _MEDICATIONREQUESTDAO_H_
#define _MEDICATIONREQUESTDAO_H_

#include "BaseDAO.h"
#include "domain/do/prescription2/MedicationRequestDO.h"

class MedicationRequestDAO : public BaseDAO
{
public:
    // Query one medical advice record within an encounter.
    PtrMedicationRequestDO selectById(const std::string& id,
        const std::string& encounterId);
    // Query and lock one medical advice record for a status transition.
    PtrMedicationRequestDO selectByIdForUpdate(const std::string& id,
        const std::string& encounterId);
    // Query and lock all medical advice identifiers in one order group.
    std::list<std::string> selectIdsByGroupForUpdate(
        const std::string& encounterId, const std::string& groupId);
    // Update status with version and expected status validation.
    int updateStatus(const MedicationRequestDO& data, const std::string& expectedStatus,
        int expectedVersion, bool updateCheckFields);
};

#endif // !_MEDICATIONREQUESTDAO_H_
