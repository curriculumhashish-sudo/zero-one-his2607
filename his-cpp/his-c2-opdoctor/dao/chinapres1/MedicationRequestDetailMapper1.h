#pragma once
#ifndef _MEDICATIONREQUESTDETAILMAPPER1_H_
#define _MEDICATIONREQUESTDETAILMAPPER1_H_

#include "Mapper.h"
#include "domain/do/chinapres1/MedicationRequestDetailDO1.h"

class MedicationRequestDetailMapper1 : public Mapper<PtrMedicationRequestDetailDO>
{
public:
    PtrMedicationRequestDetailDO mapper(ResultSet* resultSet) const override
    {
        PtrMedicationRequestDetailDO detailDO = std::make_shared<MedicationRequestDetailDO1>();
        detailDO->setId(resultSet->getString("id"));
        detailDO->setRequestId(resultSet->getString("request_id"));
        detailDO->setMedicationId(resultSet->getString("medication_id"));
        detailDO->setQuantity(resultSet->getDouble("quantity"));
        detailDO->setUnitCode(resultSet->getString("unit_code"));
        detailDO->setDose(resultSet->getDouble("dose"));
        detailDO->setDoseUnitCode(resultSet->getString("dose_unit_code"));
        detailDO->setSortNo(resultSet->getInt("sort_no"));
        return detailDO;
    }
};

#endif // !_MEDICATIONREQUESTDETAILMAPPER1_H_