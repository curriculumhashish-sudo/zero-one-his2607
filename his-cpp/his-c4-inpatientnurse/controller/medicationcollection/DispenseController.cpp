/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/18 22:00:00

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
#include "DispenseController.h"
#include "../../service/medicationcollection/DispenseService.h"
#include "dao/common/NurseStationAccessDAO.h"

DispenseSummaryPageJsonVO::Wrapper DispenseController::execQuerySummaryList(
	const DispenseSummaryQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo = DispenseSummaryPageJsonVO::createShared();

	// params validate (flow-chart step: invalid input returns to caller)
	if (!query || !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize)
		|| !inpatientnurse::validation::validDate(query->dispenseDate)
		|| !inpatientnurse::validation::validDateOnlyRange(query->startDate, query->endDate)
		|| !inpatientnurse::validation::oneOf(query->statusEnum,
			{"PENDING_SUBMIT", "SUBMITTED", "DISPENSED", "CANCELLED"}))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}

	// query summary bill list through service
	DispenseService service;
	vo->success(service.listSummary(query, payload.getId()));
	return vo;
}

DispenseDetailListJsonVO::Wrapper DispenseController::execQueryDetail(
	const DispenseDetailQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo = DispenseDetailListJsonVO::createShared();

	// Validate the required summary bill number.
	if (!query || !inpatientnurse::validation::present(query->summaryNo))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessMedicationSummary(
		payload.getId(), query->summaryNo.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}

	// Query dispense details through the service.
	DispenseService service;
	vo->success(service.listDetail(query->summaryNo));
	return vo;
}
