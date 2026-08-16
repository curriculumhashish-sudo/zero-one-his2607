#pragma once

#ifndef _PRESCRIPTION_DELETE_SUMMARY_DO_H_
#define _PRESCRIPTION_DELETE_SUMMARY_DO_H_

#include "../DoInclude.h"
#include <list>
#include <memory>

/**
 * Pass-through object for the prescription delete flow.
 * Fields mirror PrescriptionDeleteDTO.
 * Used by PrescriptionService to carry the payload from Controller
 * to the DAO layer where each entry triggers a soft-delete on
 * med_medication_request.
 */
class PrescriptionDeleteSummaryDO
{
    // Request id list (DTO field: id)
    CC_SYNTHESIZE(std::list<std::string>, medicationRequestIds, MedicationRequestIds);
    // Encounter identifier
    CC_SYNTHESIZE(string, encounterId, EncounterId);
    // Patient identifier
    CC_SYNTHESIZE(string, patientId, PatientId);
    // Tenant identifier
    CC_SYNTHESIZE(string, tenantId, TenantId);
};

typedef std::shared_ptr<PrescriptionDeleteSummaryDO> PtrPrescriptionDeleteSummaryDO;

#endif // !_PRESCRIPTION_DELETE_SUMMARY_DO_H_