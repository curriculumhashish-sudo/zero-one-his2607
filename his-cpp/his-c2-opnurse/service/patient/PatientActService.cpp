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

oatpp::List<PatientActDTO::Wrapper> PatientActService::queryServiceRequests(
	const std::string& patientId, const std::string& encounterId,
	const std::string& categoryEnum, const std::string& statusEnum,
	int performFlag)
{
	PatientActDAO dao;
	auto results = dao.queryServiceRequests(patientId, encounterId, categoryEnum, statusEnum, performFlag);

	auto dtos = oatpp::List<PatientActDTO::Wrapper>::createShared();
	for (auto& res : results) {
		auto dto = PatientActDTO::createShared();
		dto->itemId = res.request.getId();
		dto->requestId = res.requestId;
		dto->requestType = res.request.getCategoryEnum();
		dto->requestTypeName = res.requestTypeName;
		dto->orderContent = res.request.getEntrustContent();
		dto->specification = res.specification;
		dto->status = res.request.getStatusEnum();
		dto->statusName = res.statusName;
		dto->plannedCount = res.plannedCount;
		dto->executedCount = res.executedCount;
		dto->executorName = res.executorName;
		dto->executedAt = res.executedAt;
		dto->version = res.version;
		dto->printTypes = res.printTypes;
		dtos->push_back(dto);
	}

	return dtos;
}
