#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/24 18:30:00

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
#ifndef _PATIENTQUEUEDAO_H_
#define _PATIENTQUEUEDAO_H_

#include "BaseDAO.h"
#include "domain/do/patient/AppointmentQueueDO.h"
#include "domain/do/patient/PatientDO.h"

/**
 * Combined result for patient queue query (JOIN wkf_appointment_queue + adm_patient + adm_encounter + adm_department).
 */
struct PatientQueueResult
{
	AppointmentQueueDO queue;
	PatientDO patient;
	// Extra fields from joined tables
	std::string encounterId;
	std::string medicalRecordNo;
	std::string visitNo;
	std::string registeredAt;
	std::string departmentName;
	std::string visitTypeName;
	std::string statusEnum;
	std::string statusName;
	std::string paymentTypeName;
	std::string priorityName;
	std::string riskHint;
	int pendingCount;
};

/**
 * ResultSet mapper for PatientQueueResult.
 */
class PatientQueueMapper : public Mapper<PatientQueueResult>
{
public:
	PatientQueueResult mapper(ResultSet* rs) const override;
};

/**
 * DAO for querying patient queue data.
 */
class PatientQueueDAO : public BaseDAO
{
public:
	/*
	 * Query patient queue list with patient info.
	 * @param name optional patient name filter
	 * @param date optional visit date filter
	 * @param tenantId tenant ID filter
	 * @param pageIndex page index (1-based)
	 * @param pageSize page size
	 * @return list of PatientQueueResult
	 */
	std::list<PatientQueueResult> queryPatientQueue(
		const std::string& name,
		const std::string& date,
		const std::string& tenantId,
		int pageIndex,
		int pageSize);

	/*
	 * Count total patient queue records.
	 * @param name optional patient name filter
	 * @param date optional visit date filter
	 * @param tenantId tenant ID filter
	 * @return total count
	 */
	uint64_t countPatientQueue(const std::string& name, const std::string& date, const std::string& tenantId);
};

#endif // !_PATIENTQUEUEDAO_H_
