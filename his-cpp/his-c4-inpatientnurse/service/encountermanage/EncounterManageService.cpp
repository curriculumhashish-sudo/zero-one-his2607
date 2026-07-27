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

PagePatientQueueDTO::Wrapper EncounterManageService::listPatients(const PagePatientQueueQuery::Wrapper& query)
{
	auto page=PagePatientQueueDTO::createShared(); page->rows=oatpp::List<PatientQueueItemDTO::Wrapper>::createShared();
	page->pageIndex=query->pageIndex; page->pageSize=query->pageSize;
	EncounterManageDAO dao; page->total=dao.countPatients(query); page->calcPages();
	for (auto& row:dao.selectPatients(query)) page->addData(row);
	return page;
}

PatientStatisticsDTO::Wrapper EncounterManageService::getStatistics(const PatientStatisticsQuery::Wrapper& query)
{
	auto dto=PatientStatisticsDTO::createShared(); EncounterManageDAO dao;
	dto->totalCount=dao.countPatients(query); dto->pendingAuditCount=dao.countPendingAudit(query); dto->pendingExecuteCount=dao.countPendingExecute(query);
	return dto;
}
