#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:21:55

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
#ifndef _PATIENT_QUEUE_MAPPER_H_
#define _PATIENT_QUEUE_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/cycle_control/PatientQueueDO.h"

/**
 * 将多表JOIN查询结果映射为PatientQueueDO。
 * SQL查询中的列别名需与下方getString("alias")调用一致。
 */
class PatientQueueMapper : public Mapper<PatientQueueDO>
{
public:
	PatientQueueDO mapper(ResultSet* resultSet) const override
	{
		PatientQueueDO data;
		data.setEncounterId(resultSet->getString("encounter_id"));
		data.setPatientId(resultSet->getString("patient_id"));
		data.setPatientName(resultSet->getString("patient_name"));
		data.setGender(resultSet->getString("gender"));
		data.setAge(resultSet->getInt("age"));
		data.setDiagnosisSummary(resultSet->getString("diagnosis_summary"));
		data.setBedName(resultSet->getString("bed_name"));
		data.setBedId(resultSet->getString("bed_id"));
		data.setWardId(resultSet->getString("ward_id"));
		data.setWardName(resultSet->getString("ward_name"));
		data.setHouseName(resultSet->getString("house_name"));
		data.setAccountBalance(resultSet->getDouble("account_balance"));
		data.setStatusEnum(resultSet->getString("status_enum"));
		data.setEncounterBusNo(resultSet->getString("encounter_bus_no"));
		data.setAdmitTime(resultSet->getString("admit_time"));
		data.setOrganizationId(resultSet->getString("organization_id"));
		data.setOrganizationName(resultSet->getString("organization_name"));
		return data;
	}
};

/**
 * 返回 shared_ptr<PatientQueueDO> 的映射器。
 */
class PtrPatientQueueMapper : public Mapper<PtrPatientQueueDO>
{
public:
	PtrPatientQueueDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<PatientQueueDO>();
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setPatientName(resultSet->getString("patient_name"));
		data->setGender(resultSet->getString("gender"));
		data->setAge(resultSet->getInt("age"));
		data->setDiagnosisSummary(resultSet->getString("diagnosis_summary"));
		data->setBedName(resultSet->getString("bed_name"));
		data->setBedId(resultSet->getString("bed_id"));
		data->setWardId(resultSet->getString("ward_id"));
		data->setWardName(resultSet->getString("ward_name"));
		data->setHouseName(resultSet->getString("house_name"));
		data->setAccountBalance(resultSet->getDouble("account_balance"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setEncounterBusNo(resultSet->getString("encounter_bus_no"));
		data->setAdmitTime(resultSet->getString("admit_time"));
		data->setOrganizationId(resultSet->getString("organization_id"));
		data->setOrganizationName(resultSet->getString("organization_name"));
		return data;
	}
};

#endif // !_PATIENT_QUEUE_MAPPER_H_
