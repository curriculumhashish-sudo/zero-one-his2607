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
#include "BedService.h"
#include "../../dao/admissiontransfer/BedDAO.h"
#include "dao/common/NurseStationAccessDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

BedPageDTO::Wrapper BedService::listFree(
	const BedQuery::Wrapper& query, const std::string& userId)
{
    auto pages = BedPageDTO::createShared();
    pages->pageIndex = query->pageIndex;
    pages->pageSize = query->pageSize;
    pages->total = static_cast<int64_t>(0);
    pages->pages = static_cast<int64_t>(0);
    pages->rows = oatpp::List<BedDTO::Wrapper>::createShared();

    BedDAO dao;
	uint64_t count = dao.countFree(query, userId);
    if (count <= 0) {
        return pages;
    }

    pages->total = count;
    pages->calcPages();
	auto result = dao.selectFreeWithPage(query, userId);
	for (auto& item : result) pages->addData(item);
    return pages;
}

std::string BedService::assignBed(const BedAssignAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	BedDAO dao;
	const std::string appointmentId = dto->appointment_id.getValue("");
	const std::string patientId = dto->patient_id.getValue("");
	const std::string bedId = dto->location_id.getValue("");
	const std::string operatorId = payload.getId();
	const std::string resolvedEncounterId = dao.resolveEncounterId(appointmentId, patientId);
	if (resolvedEncounterId.empty()
		|| !NurseStationAccessDAO().canAccessEncounter(operatorId, resolvedEncounterId))
		return "";
	dao.getSqlSession()->beginTransaction();
	try {
		const std::string encounterId = dao.resolveEncounterId(appointmentId, patientId);
		if (encounterId != resolvedEncounterId)
			throw std::runtime_error("Assignment context changed.");
		const std::string status = dao.selectEncounterStatusForUpdate(encounterId);
		const std::string activeBed = dao.selectActiveBedForUpdate(encounterId);
		if (activeBed == bedId) {
			dao.getSqlSession()->commitTransaction();
			return encounterId;
		}
		const auto operationalStatus = dao.selectBedOperationalStatusForUpdate(bedId);
		const bool bedAvailable = operationalStatus == "VACANT"
			|| operationalStatus == "AVAILABLE" || operationalStatus == "NOT_IN_USE";
		if (status != "REGISTERED" || !activeBed.empty() || !bedAvailable
			|| dao.occupyBed(bedId, operatorId) != 1
			|| dao.createBedTrack(encounterId, bedId, operatorId) != 1
			|| dao.markEncounterAdmitted(encounterId, operatorId) != 1)
			throw std::runtime_error("Bed assignment conflict.");
		dao.consumeReservation(appointmentId, patientId, bedId, operatorId);
		dao.getSqlSession()->commitTransaction();
		return encounterId;
	}
	catch (...) {
		dao.getSqlSession()->rollbackTransaction();
		return "";
	}
}
