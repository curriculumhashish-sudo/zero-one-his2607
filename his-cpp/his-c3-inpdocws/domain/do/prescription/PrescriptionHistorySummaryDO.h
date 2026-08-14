#pragma once

#ifndef _PRESCRIPTION_HISTORY_SUMMARY_DO_H_
#define _PRESCRIPTION_HISTORY_SUMMARY_DO_H_

#include "../DoInclude.h"
#include <cstdint>

class PrescriptionHistorySummaryDO
{
    // Prescription number
    CC_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
    // prescription group id
    CC_SYNTHESIZE(string, prescriptionGroupKey, PrescriptionGroupKey);
    // Encounter identifier
    CC_SYNTHESIZE(string, encounterId, EncounterId);
    // Earliest authored time
    CC_SYNTHESIZE(string, authoredTime, AuthoredTime);
    // Medication count
    CC_SYNTHESIZE(uint64_t, medicationCount, MedicationCount);
    // Medication names JSON
    CC_SYNTHESIZE(string, medicationNamesJson, MedicationNamesJson);
};

typedef std::shared_ptr<PrescriptionHistorySummaryDO>
PtrPrescriptionHistorySummaryDO;

#endif // !_PRESCRIPTION_HISTORY_SUMMARY_DO_H_