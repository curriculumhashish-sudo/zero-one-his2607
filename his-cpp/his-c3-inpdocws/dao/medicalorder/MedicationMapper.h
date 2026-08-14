#pragma once
#ifndef _MEDICATION_MAPPER_
#define _MEDICATION_MAPPER_

#include "Mapper.h"
#include "../../domain/do/medicalorder/MedicationRequestDO.h"

/**
 * ?????????
 * ?ResultSet????MedicationRequestDO??
 * ????1=id(??), 2=status_enum(????), 3=therapy_enum(????)
 */
class MedicationMapper : public Mapper<MedicationRequestDO>
{
public:
	MedicationRequestDO mapper(ResultSet* resultSet) const override
	{
		MedicationRequestDO data;
		// ?1????ID
		data.setId(resultSet->getString(1));
		// ?2???????DRAFT=???, ACTIVE=???, STOPPED=???, CANCELLED=????
		data.setStatusEnum(resultSet->getString(2));
		// ?3???????LONG_TERM=????, TEMPORARY=?????
		data.setTherapyEnum(resultSet->getString(3));
		return data;
	}
};

/**
 * ?????????????
 * ?MedicationMapper????????shared_ptr??executeQueryOne??
 */
class PtrMedicationMapper : public Mapper<PtrMedicationRequestDO>
{
public:
	PtrMedicationRequestDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<MedicationRequestDO>();
		data->setId(resultSet->getString(1));
		data->setStatusEnum(resultSet->getString(2));
		data->setTherapyEnum(resultSet->getString(3));
		return data;
	}
};

#endif // !_MEDICATION_MAPPER_