#pragma once
#ifndef _PATIENT_MAPPER_H_
#define _PATIENT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/pat/PatientDO.h"

/**
 * Mapper for select-patient popup list.
 * NOTE: SqlSession::executeQuery already calls ResultSet::next(), do not call next() here.
 */
class PtrPatientSelectMapper : public Mapper<PtrPatientDO>
{
public:
	PtrPatientDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<PatientDO>();
		data->setBusNo(resultSet->getString("bus_no"));
		data->setName(resultSet->getString("name"));
		data->setGender(resultSet->getString("gender_enum"));

		if (!resultSet->isNull("birth_date"))
		{
			data->setBirthDate(resultSet->getString("birth_date"));
		}
		if (!resultSet->isNull("phone"))
		{
			data->setPhone(resultSet->getString("phone"));
		}
		if (!resultSet->isNull("id_card"))
		{
			data->setIdCard(resultSet->getString("id_card"));
		}
		return data;
	}
};

/**
 * Mapper for patient management list (GET /pat/page).
 * Only maps columns needed by the management table: name, gender, birth date, phone, id card.
 * NOTE: SqlSession::executeQuery already calls ResultSet::next(), do not call next() here.
 */
class PtrPatientPageMapper : public Mapper<PtrPatientDO>
{
public:
	PtrPatientDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<PatientDO>();
		data->setName(resultSet->getString("name"));
		data->setGender(resultSet->getString("gender_enum"));

		if (!resultSet->isNull("birth_date"))
		{
			data->setBirthDate(resultSet->getString("birth_date"));
		}
		if (!resultSet->isNull("phone"))
		{
			data->setPhone(resultSet->getString("phone"));
		}
		if (!resultSet->isNull("id_card"))
		{
			data->setIdCard(resultSet->getString("id_card"));
		}
		return data;
	}
};

#endif // !_PATIENT_MAPPER_H_
