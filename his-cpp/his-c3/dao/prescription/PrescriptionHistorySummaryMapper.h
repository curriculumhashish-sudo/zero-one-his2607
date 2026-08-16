#pragma once

#ifndef _PRESCRIPTION_HISTORY_SUMMARY_MAPPER_H_
#define _PRESCRIPTION_HISTORY_SUMMARY_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/prescription/PrescriptionHistorySummaryDO.h"

class PrescriptionHistorySummaryMapper
    : public Mapper<PrescriptionHistorySummaryDO>
{
public:
    PrescriptionHistorySummaryDO mapper(
        ResultSet* resultSet) const override
    {
        PrescriptionHistorySummaryDO data;

        data.setPrescriptionNo(
            resultSet->getString("prescription_no"));
        data.setPrescriptionGroupKey(
            resultSet->getString("prescriptionGroupKey"));
        data.setEncounterId(
            resultSet->getString("encounter_id"));
        data.setAuthoredTime(
            resultSet->getString("authored_time"));
        data.setMedicationCount(
            resultSet->getUInt64("medication_count"));
        data.setMedicationNamesJson(
            resultSet->getString("medication_names_json"));

        return data;
    }
};

#endif // !_PRESCRIPTION_HISTORY_SUMMARY_MAPPER_H_