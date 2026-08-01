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
#ifndef _PATIENTACTSERVICE_H_
#define _PATIENTACTSERVICE_H_

#include "dao/patient/PatientActDAO.h"
#include "domain/dto/patient/PatientActDTO.h"

/**
 * Service for handling patient service request (act) business logic.
 */
class PatientActService
{
public:
	/*
	 * Query service requests for a patient encounter.
	 * @param patientId patient ID
	 * @param encounterId encounter ID
	 * @param categoryEnum optional category filter (e.g. skin-test)
	 * @param statusEnum optional status filter
	 * @return list of PatientActDTO
	 */
	oatpp::List<PatientActDTO::Wrapper> queryServiceRequests(
		const std::string& patientId,
		const std::string& encounterId,
		const std::string& categoryEnum,
		const std::string& statusEnum,
		int performFlag = -1);
};

#endif // !_PATIENTACTSERVICE_H_
