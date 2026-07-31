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
#ifndef _DISEASE_DISTRIBUTION_MAPPER_
#define _DISEASE_DISTRIBUTION_MAPPER_

#include "Mapper.h"
#include "../../domain/do/data_statistics/DiseaseDistributionDO.h"

/**
 * Disease distribution result set field mapping
 * Column indices must match SELECT order in DiseaseDistributionDAO
 * SELECT order: name(1), patientCount(2), percentage(3)
 */
class DiseaseDistributionMapper : public Mapper<DiseaseDistributionDO>
{
public:
	DiseaseDistributionDO mapper(ResultSet* resultSet) const override
	{
		DiseaseDistributionDO data;
		data.setDiseaseName(resultSet->getString(1));
		data.setPatientCount(resultSet->getInt(2));
		data.setPercentage(resultSet->getDouble(3));
		return data;
	}
};

/**
 * Disease distribution result set field mapping - smart pointer version
 */
class PtrDiseaseDistributionMapper : public Mapper<PtrDiseaseDistributionDO>
{
public:
	PtrDiseaseDistributionDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<DiseaseDistributionDO>();
		data->setDiseaseName(resultSet->getString(1));
		data->setPatientCount(resultSet->getInt(2));
		data->setPercentage(resultSet->getDouble(3));
		return data;
	}
};

#endif // !_DISEASE_DISTRIBUTION_MAPPER_
