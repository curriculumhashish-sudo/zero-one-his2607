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
#include "PatientQueueService.h"
#include <ctime>
#include <sstream>

PatientQueuePageDTO::Wrapper PatientQueueService::queryPatientQueue(
	const std::string& name, const std::string& date,
	int pageIndex, int pageSize)
{
	PatientQueueDAO dao;
	// Query data
	auto results = dao.queryPatientQueue(name, date, pageIndex, pageSize);
	auto total = dao.countPatientQueue(name, date);

	// Convert DO to DTO
	auto dtos = oatpp::List<PatientQueueDTO::Wrapper>::createShared();
	for (auto& res : results) {
		auto dto = PatientQueueDTO::createShared();
		dto->name = res.patient.getName();
		dto->gender = convertGender(res.patient.getGenderEnum());
		dto->age = calcAge(res.patient.getBirthDate());
		dto->visitTime = res.queue.getStartInstant();
		dto->queueNumber = res.queue.getBusNo();
		std::string paymentType = res.paymentType;
		if (paymentType.empty()) paymentType = "\xE8\x87\xAA\xE8\xB4\xB9";
		dto->paymentType = paymentType;
		dtos->push_back(dto);
	}

	// Build page result
	auto page = PatientQueuePageDTO::createShared();
	page->pageIndex = pageIndex;
	page->pageSize = pageSize;
	page->total = static_cast<oatpp::Int64>(total);
	page->pages = (total + pageSize - 1) / pageSize;
	page->rows = dtos;

	return page;
}

int PatientQueueService::calcAge(const std::string& birthDate)
{
	if (birthDate.empty()) return 0;
	// Parse yyyy-MM-dd format
	std::istringstream iss(birthDate);
	int year = 0, month = 0, day = 0;
	char dash1, dash2;
	if (!(iss >> year >> dash1 >> month >> dash2 >> day)) return 0;

	// Get current date
	auto now = std::time(nullptr);
	auto* tm = std::localtime(&now);
	int curYear = tm->tm_year + 1900;
	int curMonth = tm->tm_mon + 1;
	int curDay = tm->tm_mday;

	int age = curYear - year;
	if (curMonth < month || (curMonth == month && curDay < day))
		age--;
	return age;
}

std::string PatientQueueService::convertGender(const std::string& genderEnum)
{
	if (genderEnum == "M" || genderEnum == "male") return "\xE7\x94\xB7";
	if (genderEnum == "F" || genderEnum == "female") return "\xE5\xA5\xB3";
	return genderEnum;
}
