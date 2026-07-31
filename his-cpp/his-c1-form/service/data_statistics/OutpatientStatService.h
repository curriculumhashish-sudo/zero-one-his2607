#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/28 00:00:00

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
#ifndef _OUTPATIENT_STAT_SERVICE_
#define _OUTPATIENT_STAT_SERVICE_

#include "domain/vo/data_statistics/OutpatientStatVO.h"
#include "domain/query/data_statistics/OutpatientTrendQuery.h"
#include "domain/query/data_statistics/DiseaseDistributionQuery.h"
#include "domain/dto/data_statistics/OutpatientTrendDataDTO.h"
#include "domain/dto/data_statistics/DiseaseDistributionDataDTO.h"

/**
 * Outpatient statistics business logic layer
 * Handles weekly/monthly visit trend aggregation and disease distribution analysis
 */
class OutpatientStatService
{
private:
	// Parse a "YYYY-MM-DD" string into struct tm
	struct tm parseDate(const std::string& dateStr);
	// Format a struct tm back to "YYYY-MM-DD"
	std::string formatDate(const struct tm& timeInfo);
	// Get Monday of the week containing the given date
	std::string getMondayOfWeek(const std::string& dateStr);
	// Get Sunday of the week containing the given date
	std::string getSundayOfWeek(const std::string& dateStr);
	// Get the first day of the month containing the given date
	std::string getFirstOfMonth(const std::string& dateStr);
	// Get the last day of the month containing the given date
	std::string getLastOfMonth(const std::string& dateStr);
	// Generate a list of all dates between start and end (inclusive)
	std::list<std::string> generateDateList(const std::string& start, const std::string& end);

public:
	// Query outpatient visit trend (daily detail within week when timeType=1, within month when timeType=2)
	OutpatientTrendDataDTO::Wrapper queryTrend(const OutpatientTrendQuery::Wrapper& query);
	// Query disease type distribution (including percentage calculation)
	DiseaseDistributionDataDTO::Wrapper queryDiseaseDistribution(const DiseaseDistributionQuery::Wrapper& query);
};

#endif // !_OUTPATIENT_STAT_SERVICE_
