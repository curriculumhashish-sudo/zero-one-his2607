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
#include "stdafx.h"
#include "OutpatientStatService.h"
#include "../../dao/data_statistics/OutpatientTrendDAO.h"
#include "../../dao/data_statistics/DiseaseDistributionDAO.h"
#include <map>
#include <set>
#include <ctime>
#include <cmath>

// ============ Date helper methods ============

struct tm OutpatientStatService::parseDate(const std::string& dateStr)
{
	struct tm timeInfo = {};
	int year = 0, month = 0, day = 0;
	sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
	timeInfo.tm_year = year - 1900;
	timeInfo.tm_mon = month - 1;
	timeInfo.tm_mday = day;
	timeInfo.tm_hour = 12; // noon to avoid DST issues
	return timeInfo;
}

std::string OutpatientStatService::formatDate(const struct tm& timeInfo)
{
	char buf[16];
	strftime(buf, sizeof(buf), "%Y-%m-%d", &timeInfo);
	return std::string(buf);
}

std::string OutpatientStatService::getMondayOfWeek(const std::string& dateStr)
{
	struct tm t = parseDate(dateStr);
	mktime(&t);
	// tm_wday: 0=Sunday, 1=Monday, ..., 6=Saturday
	int daysToMonday = (t.tm_wday == 0) ? 6 : (t.tm_wday - 1);
	t.tm_mday -= daysToMonday;
	mktime(&t);
	return formatDate(t);
}

std::string OutpatientStatService::getSundayOfWeek(const std::string& dateStr)
{
	struct tm t = parseDate(dateStr);
	mktime(&t);
	int daysToSunday = (t.tm_wday == 0) ? 0 : (7 - t.tm_wday);
	t.tm_mday += daysToSunday;
	mktime(&t);
	return formatDate(t);
}

std::string OutpatientStatService::getFirstOfMonth(const std::string& dateStr)
{
	struct tm t = parseDate(dateStr);
	t.tm_mday = 1;
	return formatDate(t);
}

std::string OutpatientStatService::getLastOfMonth(const std::string& dateStr)
{
	struct tm t = parseDate(dateStr);
	// Go to the 1st of next month, then back one day
	t.tm_mday = 1;
	if (t.tm_mon == 11) {
		t.tm_mon = 0;
		t.tm_year++;
	} else {
		t.tm_mon++;
	}
	t.tm_mday = 0; // 0th day of next month = last day of this month
	mktime(&t);
	return formatDate(t);
}

std::list<std::string> OutpatientStatService::generateDateList(const std::string& start, const std::string& end)
{
	std::list<std::string> dates;
	struct tm t = parseDate(start);
	struct tm tEnd = parseDate(end);
	mktime(&tEnd);

	while (true)
	{
		dates.push_back(formatDate(t));
		if (t.tm_year == tEnd.tm_year && t.tm_mon == tEnd.tm_mon && t.tm_mday == tEnd.tm_mday)
			break;
		t.tm_mday++;
		mktime(&t);
	}
	return dates;
}

// ============ Business logic ============

OutpatientTrendDataDTO::Wrapper OutpatientStatService::queryTrend(const OutpatientTrendQuery::Wrapper& query, int timeType)
{
	// Build response data object
	auto result = OutpatientTrendDataDTO::createShared();
	result->points = oatpp::List<oatpp::Object<OutpatientTrendItemDTO>>::createShared();
	result->unit = ZH_WORDS_GETTER("W.form.data_statistics.field.unit_person_times");

	// timeType: 1=weekly, 2=monthly
	std::string dateFrom = query->dateFrom.getValue("");

	// Determine the period range from the requested date
	std::string periodStart, periodEnd;
	if (timeType == 1)
	{
		periodStart = getMondayOfWeek(dateFrom);
		periodEnd = getSundayOfWeek(dateFrom);
	}
	else
	{
		periodStart = getFirstOfMonth(dateFrom);
		periodEnd = getLastOfMonth(dateFrom);
	}

	// Build a query with the computed period range (override original params)
	auto periodQuery = OutpatientTrendQuery::createShared();
	periodQuery->dateFrom = periodStart;
	periodQuery->dateTo = periodEnd;
	periodQuery->organizationId = query->organizationId;
	periodQuery->pageIndex = 1;
	periodQuery->pageSize = 9999; // fetch all

	// Query raw encounter records via DAO
	OutpatientTrendDAO dao;
	auto encounters = dao.selectEncounters(periodQuery);

	// Count encounters per day
	std::map<std::string, int> dailyCount;
	for (auto& enc : encounters)
	{
		std::string day = enc.getStartTime().substr(0, 10); // "YYYY-MM-DD"
		dailyCount[day]++;
	}

	// Generate the full date list and fill data points (zero for empty days)
	auto dateList = generateDateList(periodStart, periodEnd);
	for (auto& date : dateList)
	{
		auto item = OutpatientTrendItemDTO::createShared();
		item->label = date;
		item->value = dailyCount[date]; // 0 if not present
		result->points->push_back(item);
	}

	return result;
}

DiseaseDistributionDataDTO::Wrapper OutpatientStatService::queryDiseaseDistribution(const DiseaseDistributionQuery::Wrapper& query)
{
	// Build response data object
	auto result = DiseaseDistributionDataDTO::createShared();
	result->items = oatpp::List<oatpp::Object<DiseaseDistributionItemDTO>>::createShared();
	result->unit = ZH_WORDS_GETTER("W.form.data_statistics.field.unit_persons");

	// Query disease distribution data via DAO
	DiseaseDistributionDAO dao;
	auto distributions = dao.selectDistribution(query);

	if (distributions.empty())
		return result;

	// Convert DO list to DTO list using macro
	for (auto& dist : distributions)
	{
		auto item = DiseaseDistributionItemDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(item, dist,
			name, DiseaseName,
			value, PatientCount);
		result->items->push_back(item);
	}

	return result;
}
