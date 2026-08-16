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
#ifndef _OUTPATIENT_TREND_DAO_
#define _OUTPATIENT_TREND_DAO_
#include "BaseDAO.h"
#include "../../domain/do/data_statistics/GetTrendDO.h"
#include "../../domain/query/data_statistics/OutpatientTrendQuery.h"

/**
 * Outpatient visit trend data access object
 * Queries encounter records from adm_encounter table
 * Aggregation by timeType (1=weekly / 2=monthly) is handled at the Service layer
 */
class OutpatientTrendDAO : public BaseDAO
{
private:
	// Build WHERE clause dynamically from query filters (date range, organization, soft delete)
	inline std::string queryConditionBuilder(const OutpatientTrendQuery::Wrapper& query, SqlParams& params);

public:
	// Count matching encounter records
	uint64_t count(const OutpatientTrendQuery::Wrapper& query);
	// Query encounter records for trend analysis
	std::list<GetTrendDO> selectEncounters(const OutpatientTrendQuery::Wrapper& query);
};

#endif // !_OUTPATIENT_TREND_DAO_
