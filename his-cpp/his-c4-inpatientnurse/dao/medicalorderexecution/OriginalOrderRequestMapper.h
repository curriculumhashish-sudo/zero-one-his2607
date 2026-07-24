#pragma once

#ifndef _ORIGINAL_ORDER_REQUEST_MAPPER_
#define _ORIGINAL_ORDER_REQUEST_MAPPER_

#include "Mapper.h"
#include "domain/do/medicalorderexecution/OriginalOrderRequestDO.h"

class OriginalOrderRequestMapper : public Mapper<PtrOriginalOrderRequestDO>
{
public:
	PtrOriginalOrderRequestDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<OriginalOrderRequestDO>();
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setCategoryEnum(resultSet->getString("category_enum"));
		return data;
	}
};

#endif // !_ORIGINAL_ORDER_REQUEST_MAPPER_
