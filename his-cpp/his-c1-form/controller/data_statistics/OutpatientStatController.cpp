/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/20 00:00:00

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
#include "controller/data_statistics/OutpatientStatController.h"
#include "../../service/data_statistics/OutpatientStatService.h"

OutpatientTrendJsonVO::Wrapper OutpatientStatController::execQueryWeeklyTrend(const OutpatientTrendQuery::Wrapper& query)
{
	// Call Service layer to query weekly outpatient visit trend
	auto result = OutpatientStatService().queryTrend(query, 1);
	auto jvo = OutpatientTrendJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

OutpatientTrendJsonVO::Wrapper OutpatientStatController::execQueryMonthlyTrend(const OutpatientTrendQuery::Wrapper& query)
{
	// Call Service layer to query monthly outpatient visit trend
	auto result = OutpatientStatService().queryTrend(query, 2);
	auto jvo = OutpatientTrendJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

DiseaseDistributionJsonVO::Wrapper OutpatientStatController::execQueryDiseaseDistribution(const DiseaseDistributionQuery::Wrapper& query)
{
	// Call Service layer to query disease distribution
	auto result = OutpatientStatService().queryDiseaseDistribution(query);
	auto jvo = DiseaseDistributionJsonVO::createShared();
	jvo->success(result);
	return jvo;
}
