#pragma once

#ifndef _PRESCRIPTION_CURRENT_SUMMARY_DO_H_
#define _PRESCRIPTION_CURRENT_SUMMARY_DO_H_

#include "../DoInclude.h"

/**
 * Projection object for the current TCM prescription query.
 * One row represents a single prescription (grouped by prescription_no
 * and the prescription-level fields). All medication lines belonging to
 * the prescription are aggregated into medicationDetailsJson.
 * Mapped by PrescriptionCurrentSummaryMapper.
 */
class PrescriptionCurrentSummaryDO
{
    // Prescription number (m1.prescription_no)
    CC_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
    // Prescription group id
    CC_SYNTHESIZE(string, prescriptionGroupKey, PrescriptionGroupKey);
    // Prescription status enumeration value (m1.status_enum)
    CC_SYNTHESIZE(string, statusEnum, StatusEnum);
    // Long-term or short-term therapy enumeration value (m1.therapy_enum)
    CC_SYNTHESIZE(string, therapyEnum, TherapyEnum);
    // Administration method display text (t1.display)
    CC_SYNTHESIZE(string, methodDisplay, MethodDisplay);
    // Administration frequency name (c1.name)
    CC_SYNTHESIZE(string, frequencyName, FrequencyName);
    // Dispensing duration in days (m1.dispense_per_duration)
    CC_SYNTHESIZE(int, dispensePerDuration, DispensePerDuration);
    // Chinese herbs dose quantity, number of doses (m1.chinese_herbs_dose_quantity)
    CC_SYNTHESIZE(double, chineseHerbsDoseQuantity, ChineseHerbsDoseQuantity);
    // Suffering flag, whether decocted on behalf (m1.suffering_flag)
    CC_SYNTHESIZE(bool, sufferingFlag, SufferingFlag);
    // Diagnosis description (a1.description)
    CC_SYNTHESIZE(string, diagnosisDescription, DiagnosisDescription);
    // Prescription remark (m1.remark)
    CC_SYNTHESIZE(string, remark, Remark);
    // Medication lines aggregated as a JSON array.
    // Each element is a JSON array in the fixed order:
    //   [ request_id (m1.id),
    //     medication_definition_id (m2.id),
    //     medication_name (m2.name),
    //     dose (m1.dose),
    //     price_value (a2.price_value),
    //     unit_display (t2.display) ]
    CC_SYNTHESIZE(string, medicationDetailsJson, MedicationDetailsJson);
};

typedef std::shared_ptr<PrescriptionCurrentSummaryDO>
PtrPrescriptionCurrentSummaryDO;

#endif // !_PRESCRIPTION_CURRENT_SUMMARY_DO_H_
