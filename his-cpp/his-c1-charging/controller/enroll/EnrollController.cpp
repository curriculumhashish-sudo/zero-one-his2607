/*
 Copyright Zero One Star. All rights reserved.

 @Author: QQQQ
 @Date: 2026/07/19

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
#include "EnrollController.h"

EnrollPageJsonVO::Wrapper EnrollController::execQueryAll(
	const EnrollQuery::Wrapper& query
)
{
	// Build a sample list item for API documentation.
	auto item = EnrollListDTO::createShared();
	item->encounterId = "demo-encounter-001";
	item->encounterBusNo = "ZY202607190001";
	item->patientName = "Zhang San";
	item->statusEnum = "WAIT_ADMISSION";
	item->statusEnumText = "Waiting for admission";
	item->organizationName = "Internal Medicine";
	item->wardName = "Internal Medicine Ward";
	item->bedName = "01";
	item->startTime = "2026-07-19 08:00:00";

	// Build paginated sample data.
	auto rows = oatpp::List<EnrollListDTO::Wrapper>::createShared();
	rows->push_back(item);

	auto page = EnrollPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	page->total = 1;
	page->rows = rows;
	page->calcPages();

	// Build the unified JSON response.
	auto result = EnrollPageJsonVO::createShared();
	result->success(page);
	return result;
}

EnrollDetailJsonVO::Wrapper EnrollController::execQueryById(
	const String& encounterId
)
{
	// Build the unified JSON response.
	auto result = EnrollDetailJsonVO::createShared();

	// Reject an empty registration record ID.
	if (!encounterId)
	{
		result->init(nullptr, RS_PARAMS_INVALID);
		return result;
	}

	// Build sample detail data for API documentation.
	auto detail = EnrollDetailDTO::createShared();
	detail->encounterId = encounterId;
	detail->encounterBusNo = "ZY202607190001";
	detail->patientName = "Zhang San";
	detail->statusEnum = "WAIT_ADMISSION";
	detail->statusEnumText = "Waiting for admission";
	detail->organizationName = "Internal Medicine";
	detail->wardName = "Internal Medicine Ward";
	detail->bedName = "01";
	detail->startTime = "2026-07-19 08:00:00";
	detail->patientId = "demo-patient-001";
	detail->idCard = "11010119900101123X";
	detail->genderEnum = "MALE";
	detail->genderEnumText = "Male";
	detail->birthDate = "1990-01-01";
	detail->age = 36;
	detail->phone = "13800000000";
	detail->admissionCount = 1;
	detail->organizationId = "demo-organization-001";
	detail->wardId = "demo-ward-001";
	detail->bedId = "demo-bed-001";
	detail->outpatientDoctorName = "Li Ming";
	detail->attendingDoctorId = "demo-doctor-001";
	detail->attendingDoctorName = "Wang Wei";
	detail->conditionDefinitionId = "demo-condition-001";
	detail->conditionDefinitionName = "Sample diagnosis";
	detail->diagnosisDescription = "Sample diagnosis description";
	detail->admitSourceCode = "OUTPATIENT";
	detail->admitSourceCodeText = "Outpatient";
	detail->inWayCode = "NORMAL";
	detail->inWayCodeText = "Normal admission";
	detail->priorityEnum = "NORMAL";
	detail->priorityEnumText = "Normal";
	detail->contractId = "demo-contract-001";
	detail->contractName = "Self-pay";

	result->success(detail);
	return result;
}
