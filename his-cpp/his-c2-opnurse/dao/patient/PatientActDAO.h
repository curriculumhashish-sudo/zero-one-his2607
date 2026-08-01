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
#ifndef _PATIENTACTDAO_H_
#define _PATIENTACTDAO_H_

#include "BaseDAO.h"
#include "domain/do/patient/ServiceRequestDO.h"

/**
 * Extended result for service request query with joined display fields.
 */
struct ServiceRequestResult
{
	ServiceRequestDO request;
	// Extra fields from joined tables
	std::string requestId;
	std::string requestTypeName;
	std::string specification;
	std::string statusName;
	int plannedCount;
	int executedCount;
	std::string executorName;
	std::string executedAt;
	int version;
	std::string printTypes;
};

/**
 * ResultSet mapper for ServiceRequestResult.
 */
class ServiceRequestResultMapper : public Mapper<ServiceRequestResult>
{
public:
	ServiceRequestResult mapper(ResultSet* rs) const override;
};

/**
 * DAO for querying patient service request (act) data.
 */
class PatientActDAO : public BaseDAO
{
public:
	/*
	 * Query service requests for a patient encounter.
	 * @param patientId patient ID
	 * @param encounterId encounter ID
	 * @param categoryEnum optional category filter (e.g. skin-test)
	 * @param statusEnum optional status filter
	 * @return list of ServiceRequestResult
	 */
	std::list<ServiceRequestResult> queryServiceRequests(
		const std::string& patientId,
		const std::string& encounterId,
		const std::string& categoryEnum,
		const std::string& statusEnum,
		int performFlag = -1);
};

#endif // !_PATIENTACTDAO_H_
