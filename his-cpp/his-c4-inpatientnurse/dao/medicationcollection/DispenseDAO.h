#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:40:00

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
#ifndef _DISPENSE_DAO_
#define _DISPENSE_DAO_
#include "BaseDAO.h"
#include "../../domain/do/medicationcollection/DispenseSummaryDO.h"
#include "../../domain/do/medicationcollection/DispenseDetailDO.h"
#include "../../domain/query/medicationcollection/DispenseSummaryQuery.h"

/**
 * Data access of the summary drug-collection feature.
 * Table: med_medication_dispense (+ med_medication_definition for names).
 */
class DispenseDAO : public BaseDAO
{
private:
	// build WHERE condition by DispenseSummaryQuery (status/date)
	inline std::string queryConditionBuilder(const DispenseSummaryQuery::Wrapper& query, SqlParams& params);
public:
	// count distinct summary bills by condition
	uint64_t countSummary(const DispenseSummaryQuery::Wrapper& query);
	// query summary bill list with pagination (aggregated by summary_no)
	std::list<DispenseSummaryDO> selectSummaryWithPage(const DispenseSummaryQuery::Wrapper& query);
	// query dispense details by summary bill no (join medication definition)
	std::list<DispenseDetailDO> selectDetailBySummaryNo(const std::string& summaryNo);
};
#endif // !_DISPENSE_DAO_
