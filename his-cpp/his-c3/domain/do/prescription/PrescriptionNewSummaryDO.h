#pragma once

#ifndef _PRESCRIPTION_NEW_SUMMARY_DO_H_
#define _PRESCRIPTION_NEW_SUMMARY_DO_H_

#include "../DoInclude.h"
#include "../../dto/prescription/PrescriptionNewPrDTO.h"
#include <list>
#include <memory>

/**
 * Pass-through object for the new-prescription insert flow.
 * Fields mirror PrescriptionNewDTO.
 * Used by PrescriptionService to carry the payload from Controller
 * to the DAO layer where each entry is converted into a
 * MedMedicationRequestDO row for insertion.
 */
class PrescriptionNewSummaryDO
{
    // Encounter identifier (DTO field: id)
    CC_SYNTHESIZE(string, encounterId, EncounterId);
    // Patient identifier
    CC_SYNTHESIZE(string, patientId, PatientId);
    // tenant id
    CC_SYNTHESIZE(string, tenantId, TenantId);
    // create time
    CC_SYNTHESIZE(string, createAt, CreateAt);
    // Prescription entry list (each entry becomes one or more rows in med_medication_request)
    CC_SYNTHESIZE(std::list<PrescriptionNewPrDTO::Wrapper>, prescriptions, Prescriptions);
};

typedef std::shared_ptr<PrescriptionNewSummaryDO> PtrPrescriptionNewSummaryDO;

#endif // !_PRESCRIPTION_NEW_SUMMARY_DO_H_