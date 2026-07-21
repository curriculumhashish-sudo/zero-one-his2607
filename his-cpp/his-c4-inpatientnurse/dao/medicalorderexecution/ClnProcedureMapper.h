#pragma once

#ifndef _CLN_PROCEDURE_MAPPER_
#define _CLN_PROCEDURE_MAPPER_

#include "Mapper.h"
#include "domain/do/medicalorderexecution/ClnProcedureDO.h"

class ClnProcedureSlotStateMapper : public Mapper<PtrClnProcedureDO>
{
public:
	PtrClnProcedureDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<ClnProcedureDO>();

		data->setId(resultSet->getString("id"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setOccurrenceSlotActive(resultSet->getBoolean("occurrence_slot_active"));

		return data;
	}
};

class ClnProcedureCompletedMapper : public Mapper<PtrClnProcedureDO>
{
public:
	PtrClnProcedureDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<ClnProcedureDO>();

		data->setId(resultSet->getString("id"));
		if (!resultSet->isNull("tenant_id")) data->setTenantId(resultSet->getString("tenant_id"));
		data->setVersion(resultSet->getInt("version"));
		data->setRequestId(resultSet->getString("request_id"));
		data->setRequestTable(resultSet->getString("request_table"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setCategoryEnum(resultSet->getString("category_enum"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setPatientId(resultSet->getString("patient_id"));
		if (!resultSet->isNull("occurrence_time")) data->setOccurrenceTime(resultSet->getString("occurrence_time"));
		if (!resultSet->isNull("recorder_id")) data->setRecorderId(resultSet->getString("recorder_id"));
		if (!resultSet->isNull("org_id")) data->setOrgId(resultSet->getString("org_id"));
		if (!resultSet->isNull("location_id")) data->setLocationId(resultSet->getString("location_id"));
		if (!resultSet->isNull("group_id")) data->setGroupId(resultSet->getString("group_id"));
		if (!resultSet->isNull("refund_id")) data->setRefundId(resultSet->getString("refund_id"));
		data->setOccurrenceSlotActive(resultSet->getBoolean("occurrence_slot_active"));

		return data;
	}
};

#endif // !_CLN_PROCEDURE_MAPPER_
