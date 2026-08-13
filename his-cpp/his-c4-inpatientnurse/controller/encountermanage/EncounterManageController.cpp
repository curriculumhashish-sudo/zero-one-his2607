/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#include "EncounterManageController.h"
#include "service/encountermanage/EncounterManageService.h"

WardTreeJsonVO::Wrapper EncounterManageController::execQueryWardTree(const PayloadDTO& payload)
{
	auto vo = WardTreeJsonVO::createShared();
	vo->success(EncounterManageService().getWardTree(payload.getId()));
	return vo;
}

PatientStatisticsJsonVO::Wrapper EncounterManageController::execQueryPatientStatistics(
const PatientStatisticsQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo=PatientStatisticsJsonVO::createShared();
	if (!query
		|| !inpatientnurse::validation::validCsv(query->wardIds, 50)
		|| !inpatientnurse::validation::validCsv(query->houseIds, 50)
		|| !inpatientnurse::validation::validDate(query->nurseStationExecutionDate)
		|| !inpatientnurse::validation::validDateRange(query->startDate, query->endDate)
		|| !inpatientnurse::validation::oneOf(query->nurseStationTaskFilter,
			{"pending-audit", "pending-execute"})
		|| !inpatientnurse::validation::oneOf(query->nurseStationStatusFilter,
			{"REGISTERED", "IN_PROGRESS", "ON_HOLD", "PENDING", "COMPLETED",
			 "DISCHARGED", "SETTLED_DISCHARGED"}))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	vo->success(EncounterManageService().getStatistics(query, payload.getId()));
	return vo;
}

PagePatientQueueJsonVO::Wrapper EncounterManageController::execQueryPatientQueue(
	const PagePatientQueueQuery::Wrapper& query, const PayloadDTO& payload)
{
	auto vo=PagePatientQueueJsonVO::createShared();
	if (!query || !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize)
		|| !inpatientnurse::validation::validCsv(query->wardIds, 50)
		|| !inpatientnurse::validation::validCsv(query->houseIds, 50)
		|| !inpatientnurse::validation::validDate(query->nurseStationExecutionDate)
		|| !inpatientnurse::validation::validDateRange(query->startDate, query->endDate)
		|| !inpatientnurse::validation::oneOf(query->nurseStationTaskFilter,
			{"pending-audit", "pending-execute"})
		|| !inpatientnurse::validation::oneOf(query->nurseStationStatusFilter,
			{"REGISTERED", "IN_PROGRESS", "ON_HOLD", "PENDING", "COMPLETED",
			 "DISCHARGED", "SETTLED_DISCHARGED"})
		|| !inpatientnurse::validation::oneOf(query->sortBy, {"startTime", "patientName", "bedName"})
		|| !inpatientnurse::validation::oneOf(query->sortDirection, {"ASC", "DESC"}))
	{
		vo->init(nullptr,RS_PARAMS_INVALID);
		return vo;
	}
	vo->success(EncounterManageService().listPatients(query, payload.getId())); return vo;
}
