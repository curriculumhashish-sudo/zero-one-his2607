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
#include "PatientActService.h"
#include <sstream>

oatpp::List<PatientActDTO::Wrapper> PatientActService::queryServiceRequests(
	const std::string& patientId, const std::string& encounterId,
	const std::string& categoryEnum, const std::string& statusEnum)
{
	PatientActDAO dao;
	auto results = dao.queryServiceRequests(patientId, encounterId, categoryEnum, statusEnum);

	auto dtos = oatpp::List<PatientActDTO::Wrapper>::createShared();
	for (auto& s : results) {
		auto dto = PatientActDTO::createShared();
		dto->status = s.getStatusEnum();
		dto->orderContent = s.getEntrustContent();
		std::string price = s.getTotalPrice();
		if (!price.empty()) {
			std::ostringstream oss;
			oss << "\xE5\xB7\xB2\xE7\xBB\x93\xE7\xAE\x97\xEF\xBC\x9B" << price << "\xE5\x85\x83";
			dto->amount = oss.str();
		}
		else {
			dto->amount = "\xE6\x9C\xAA\xE7\xBB\x93\xE7\xAE\x97";
		}
		dto->isAct = s.getPerformFlag();
		dtos->push_back(dto);
	}

	return dtos;
}
