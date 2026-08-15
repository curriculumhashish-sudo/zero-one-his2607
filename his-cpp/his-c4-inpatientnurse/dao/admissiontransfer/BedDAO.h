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
#pragma once
#ifndef _BED_DAO_
#define _BED_DAO_
#include "BaseDAO.h"
#include "../../domain/do/admissiontransfer/LocationDO.h"
#include "../../domain/do/admissiontransfer/BedReservationDO.h"
#include "../../domain/query/admissiontransfer/BedQuery.h"
#include "domain/dto/admissiontransfer/BedDTO.h"

// Bed DAO: query free beds + reserve(assign) a bed
class BedDAO : public BaseDAO
{
private:
    // Build the shared WHERE clause (free-bed condition)
	inline std::string queryConditionBuilder(const BedQuery::Wrapper& query,
		const std::string& userId, SqlParams& params);
public:
    // Count free beds
	uint64_t countFree(const BedQuery::Wrapper& query, const std::string& userId);
    // Query free beds with pagination
	std::list<BedDTO::Wrapper> selectFreeWithPage(
		const BedQuery::Wrapper& query, const std::string& userId);
    bool canReserve(
        const std::string& patientId,
        const std::string& locationId,
        const std::string& intendedAdmissionDate);
    // Insert a reservation record (assign). Returns affected rows.
    int reserve(const BedReservationDO& data);
	std::string resolveEncounterId(
		const std::string& appointmentId, const std::string& patientId);
	std::string selectEncounterStatusForUpdate(const std::string& encounterId);
	std::string selectActiveBedForUpdate(const std::string& encounterId);
	std::string selectBedOperationalStatusForUpdate(const std::string& bedId);
	int occupyBed(const std::string& bedId, const std::string& operatorId);
	int createBedTrack(const std::string& encounterId, const std::string& bedId,
		const std::string& operatorId);
	int markEncounterAdmitted(const std::string& encounterId,
		const std::string& operatorId);
	int consumeReservation(const std::string& appointmentId,
		const std::string& patientId, const std::string& bedId,
		const std::string& operatorId);
};

#endif // !_BED_DAO_
