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
#ifndef _DISEASE_DISTRIBUTION_DAO_
#define _DISEASE_DISTRIBUTION_DAO_
#include "BaseDAO.h"
#include "../../domain/do/data_statistics/DiseaseDistributionDO.h"
#include "../../domain/query/data_statistics/DiseaseDistributionQuery.h"

/**
 * Disease distribution statistics data access object
 * Crosses four tables with JOIN and GROUP BY aggregation:
 *   adm_encounter -> adm_encounter_diagnosis -> cln_condition -> cln_condition_definition
 */
class DiseaseDistributionDAO : public BaseDAO
{
private:
	// Dynamically build WHERE clause based on query filters (date range, organization)
	inline std::string queryConditionBuilder(const DiseaseDistributionQuery::Wrapper& query, SqlParams& params);

public:
	// Count matching disease types
	uint64_t count(const DiseaseDistributionQuery::Wrapper& query);
	// Query disease distribution data (grouped by disease name, with percentage calculation)
	std::list<DiseaseDistributionDO> selectDistribution(const DiseaseDistributionQuery::Wrapper& query);
};

#endif // !_DISEASE_DISTRIBUTION_DAO_
