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
#include "dao/common/NurseStationAccessDAO.h"

CostDetailPageJsonVO::Wrapper CostDetailController::execQueryAll(
	const CostDetailQuery::Wrapper& query, const PayloadDTO& payload) {
	auto jvo = CostDetailPageJsonVO::createShared();
	if (!query || !inpatientnurse::validation::present(query->patientId)
		|| !inpatientnurse::validation::present(query->encounterId)
		|| !inpatientnurse::validation::present(query->startDate)
		|| !inpatientnurse::validation::present(query->endDate)
		|| !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize)
		|| query->pageSize.getValue(0) > 100
		|| !inpatientnurse::validation::validDateOnlyRange(query->startDate, query->endDate))
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), query->encounterId.getValue(""),
		query->patientId.getValue(""))) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
	auto result = CostDetailService().queryDetails(query);
	jvo->success(result);
	return jvo;
}

CostSummaryListJsonVO::Wrapper CostDetailController::execSummaryByEncounter(
	const CostSummaryQuery::Wrapper& query, const PayloadDTO& payload) {
	auto jvo = CostSummaryListJsonVO::createShared();
	if (!query || !inpatientnurse::validation::present(query->patientId)
		|| !inpatientnurse::validation::present(query->encounterId)
		|| !inpatientnurse::validation::present(query->startDate)
		|| !inpatientnurse::validation::present(query->endDate)
		|| !inpatientnurse::validation::validDateOnlyRange(query->startDate, query->endDate))
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), query->encounterId.getValue(""),
		query->patientId.getValue(""))) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
	auto result = CostDetailService().querySummary(query);
	jvo->success(result);
	return jvo;
}
