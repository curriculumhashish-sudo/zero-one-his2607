#pragma once
#ifndef _AdmEncounterMapper_H_
#define _AdmEncounterMapper_H_

#include "Mapper.h"
#include "../../domain/do/adm/AdmEncounterDO.h"

/**
 * Result-set mapper for adm_encounter table.
 * Converts a single ResultSet row into an AdmEncounterDO.
 */
class AdmEncounterMapper : public Mapper<PtrAdmEncounterDO>
{
public:
	PtrAdmEncounterDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<AdmEncounterDO>();
		data->setId(resultSet->getString("id"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		return data;
	}
};

#endif // !_AdmEncounterMapper_H_
