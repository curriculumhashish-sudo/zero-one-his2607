#pragma once

#ifndef _PRESCRIPTION_CURRENT_SUMMARY_MAPPER_H_
#define _PRESCRIPTION_CURRENT_SUMMARY_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/prescription/PrescriptionCurrentSummaryDO.h"

/**
 * Maps one row of the current TCM prescription query to
 * PrescriptionCurrentSummaryDO.
 * Used by SqlSession::executeQuery<PrescriptionCurrentSummaryDO>().
 *
 * The source SQL must expose the following column aliases:
 *   prescription_no, status_enum, therapy_enum, method_display,
 *   frequency_name, dispense_per_duration, chinese_herbs_dose_quantity,
 *   suffering_flag, diagnosis_description, remark, medication_details_json
 */
class PrescriptionCurrentSummaryMapper
    : public Mapper<PrescriptionCurrentSummaryDO>
{
public:
    PrescriptionCurrentSummaryDO mapper(
        ResultSet* resultSet) const override
    {
        PrescriptionCurrentSummaryDO data;

        data.setPrescriptionNo(
            resultSet->getString("prescription_no"));
        data.setPrescriptionGroupKey(
            resultSet->getString("prescriptionGroupKey"));
        data.setStatusEnum(
            resultSet->getString("status_enum"));
        data.setTherapyEnum(
            resultSet->getString("therapy_enum"));
        data.setMethodDisplay(
            resultSet->getString("method_display"));
        data.setFrequencyName(
            resultSet->getString("frequency_name"));
        data.setDispensePerDuration(
            resultSet->getInt("dispense_per_duration"));
        data.setChineseHerbsDoseQuantity(
            resultSet->getDouble("chinese_herbs_dose_quantity"));
        data.setSufferingFlag(
            resultSet->getBoolean("suffering_flag"));
        data.setDiagnosisDescription(
            resultSet->getString("diagnosis_description"));
        data.setRemark(
            resultSet->getString("remark"));
        data.setMedicationDetailsJson(
            resultSet->getString("medication_details_json"));

        return data;
    }
};

#endif // !_PRESCRIPTION_CURRENT_SUMMARY_MAPPER_H_
