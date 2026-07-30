#pragma once
#ifndef PATIENTFINISHMAPPER_H_
#define PATIENTFINISHMAPPER_H_

#include "domain/do/cycleCtrl1/AdmEncounterDO.h"
#include "Mapper.h"

// 单条就诊查询映射Mapper
class PatientFinishMapper : public Mapper<PtrAdmEncounterDO>
{
public:
    PtrAdmEncounterDO mapper(ResultSet* resultSet) const override
    {
        PtrAdmEncounterDO encDO = std::make_shared<AdmEncounterDO>();
        encDO->setId(resultSet->getString("id"));
		encDO->setTenantId(resultSet->getString("tenant_id"));
        encDO->setStatusEnum(resultSet->getString("status_enum"));
        encDO->setIsDeleted(resultSet->getInt64("is_deleted"));
        return encDO;
    }

};

#endif // !PATIENTFINISHMAPPER_H_