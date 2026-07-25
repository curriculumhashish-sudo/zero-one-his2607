/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#pragma once

#ifndef _CLN_PROCEDURE_MAPPER_
#define _CLN_PROCEDURE_MAPPER_

#include "Mapper.h"
#include "domain/do/common/ProcedureDO.h"

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
