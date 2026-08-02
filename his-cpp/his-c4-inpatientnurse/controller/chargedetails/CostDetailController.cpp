/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
#include "CostDetailController.h"
#include "service/chargedetails/CostDetailService.h"

CostDetailPageJsonVO::Wrapper CostDetailController::execQueryAll(const CostDetailQuery::Wrapper& query) {
	auto jvo = CostDetailPageJsonVO::createShared();
	auto result = CostDetailService().queryDetails(query);
	jvo->success(result);
	return jvo;
}

CostSummaryListJsonVO::Wrapper CostDetailController::execSummaryByEncounter(const CostSummaryQuery::Wrapper& query) {
	auto jvo = CostSummaryListJsonVO::createShared();
	auto result = CostDetailService().querySummary(query);
	jvo->success(result);
	return jvo;
}
