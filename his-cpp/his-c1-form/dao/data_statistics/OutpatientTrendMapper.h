#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/27 00:00:00

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
#ifndef _OUTPATIENT_TREND_MAPPER_
#define _OUTPATIENT_TREND_MAPPER_

#include "Mapper.h"
#include "../../domain/do/data_statistics/GetTrendDO.h"

/**
 * Outpatient trend result set field mapping (adm_encounter table)
 * Column indices must match SELECT order in OutpatientTrendDAO
 * SELECT order: id(1), patient_id(2), start_time(3), end_time(4),
 *                organization_id(5), is_deleted(6), status_enum(7)
 */
class OutpatientTrendMapper : public Mapper<GetTrendDO>
{
public:
	GetTrendDO mapper(ResultSet* resultSet) const override
	{
		GetTrendDO data;
		data.setId(resultSet->getString(1));
		data.setPatientId(resultSet->getString(2));
		data.setStartTime(resultSet->getString(3));
		data.setEndTime(resultSet->getString(4));
		data.setOrganizationId(resultSet->getString(5));
		data.setIsDeleted(resultSet->getInt(6));
		data.setStatusNum(resultSet->getString(7));
		return data;
	}
};

/**
 * Outpatient trend result set field mapping - smart pointer version
 */
class PtrOutpatientTrendMapper : public Mapper<PtrGetTrendDO>
{
public:
	PtrGetTrendDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<GetTrendDO>();
		data->setId(resultSet->getString(1));
		data->setPatientId(resultSet->getString(2));
		data->setStartTime(resultSet->getString(3));
		data->setEndTime(resultSet->getString(4));
		data->setOrganizationId(resultSet->getString(5));
		data->setIsDeleted(resultSet->getInt(6));
		data->setStatusNum(resultSet->getString(7));
		return data;
	}
};

#endif // !_OUTPATIENT_TREND_MAPPER_
