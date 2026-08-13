#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/08/12 00:00:00

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
#ifndef _NURSE_STATION_ACCESS_DAO_H_
#define _NURSE_STATION_ACCESS_DAO_H_

#include "BaseDAO.h"

class NurseStationAccessDAO : public BaseDAO
{
public:
	bool canAccessEncounter(const std::string& userId, const std::string& encounterId);
	bool canAccessEncounterPatient(
		const std::string& userId, const std::string& encounterId,
		const std::string& patientId);
	bool canAccessBed(const std::string& userId, const std::string& bedId);
	bool canAccessMedicationSummary(
		const std::string& userId, const std::string& summaryNo);
	bool ownsMedicationIdempotency(
		const std::string& userId, const std::string& idempotencyKey,
		const std::string& operation);
};

#endif // !_NURSE_STATION_ACCESS_DAO_H_
