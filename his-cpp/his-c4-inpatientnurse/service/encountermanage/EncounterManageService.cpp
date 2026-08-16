/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "EncounterManageService.h"
#include "dao/encountermanage/EncounterManageDAO.h"

PagePatientQueueDTO::Wrapper EncounterManageService::listPatients(
	const PagePatientQueueQuery::Wrapper& query, const std::string& userId)
{
	auto page=PagePatientQueueDTO::createShared(); page->rows=oatpp::List<PatientQueueItemDTO::Wrapper>::createShared();
	page->pageIndex=query->pageIndex; page->pageSize=query->pageSize;
	EncounterManageDAO dao; page->total=dao.countPatients(query, userId); page->calcPages();
	for (auto& row:dao.selectPatients(query, userId)) page->addData(row);
	return page;
}

PatientStatisticsDTO::Wrapper EncounterManageService::getStatistics(
	const PatientStatisticsQuery::Wrapper& query, const std::string& userId)
{
	auto dto=PatientStatisticsDTO::createShared(); EncounterManageDAO dao;
	dto->totalCount=dao.countPatients(query, userId);
	dto->pendingAuditCount=dao.countPendingAudit(query, userId);
	dto->pendingExecuteCount=dao.countPendingExecute(query, userId);
	return dto;
}

WardTreeDTO::Wrapper EncounterManageService::getWardTree(const std::string& userId)
{
	auto result = WardTreeDTO::createShared();
	result->wardNodes = oatpp::List<WardNodeDTO::Wrapper>::createShared();
	EncounterManageDAO dao;
	for (auto& ward : dao.selectWards(userId)) {
		ward->houseNodes = oatpp::List<HouseNodeDTO::Wrapper>::createShared();
		for (auto& house : dao.selectHouses(ward->wardId.getValue(""), userId)) ward->houseNodes->push_back(house);
		result->wardNodes->push_back(ward);
	}
	return result;
}
