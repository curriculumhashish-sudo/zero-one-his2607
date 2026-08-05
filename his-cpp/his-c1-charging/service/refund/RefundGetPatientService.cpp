/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/26 00:00:00

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
#include "RefundGetPatientService.h"
#include "../../dao/refund/RefundGetPatientDAO.h"
#include <ctime>

// Prevent macro conflicts
#include "NacosClient.h"

// Paginated query for refund patient queue
// Input: query (keyword, queueTabCode, pageIndex, pageSize)
// Output: RefundGetPatientPageDTO with content[], totalCount, pageNo, pageSize
RefundGetPatientPageDTO::Wrapper RefundGetPatientService::listAll(const RefundGetPatientQuery::Wrapper& query)
{
    // Build page response skeleton
    auto pages = RefundGetPatientPageDTO::createShared();
    // Initialize content as empty list — ensures JSON always contains "content":[...]
    pages->content = oatpp::List<RefundGetPatientDTO::Wrapper>::createShared();
    pages->pageNo = query->pageIndex;
    pages->pageSize = query->pageSize;

    // Query total count — return empty page if none found
    RefundGetPatientDAO dao;
    uint64_t count = dao.count(query);
    if (count <= 0) {
        pages->totalCount = "0";
        return pages;
    }
    pages->totalCount = std::to_string(count);

    // Get current year for age calculation
    time_t now = time(nullptr);
    struct tm tmInfo;
#ifdef LINUX
    localtime_r(&now, &tmInfo);
#else
    localtime_s(&tmInfo, &now);
#endif
    int currentYear = tmInfo.tm_year + 1900;

    // Query paginated data and convert DO → DTO
    std::list<RefundGetPatientDO> result = dao.selectWithPage(query);
    for (RefundGetPatientDO& sub : result) {
        auto dto = RefundGetPatientDTO::createShared();
        // Map all 14 fields: DTO field name → DO getter suffix
        ZO_STAR_DOMAIN_DO_TO_DTO(
            dto, sub,
            encounterId, EncounterId,
            encounterBusNo, EncounterBusNo,
            patientId, PatientId,
            patientName, PatientName,
            genderCode, GenderCode,
            genderEnumText, GenderEnumText,
            // ageText overridden below — SQL returns raw birth_date due to
            // MySQL C++ Connector prepared-statement limitation with date functions
            ageText, AgeText,
            queueLabelCode, QueueLabelCode,
            chargeItemStatusSummary, ChargeItemStatusSummary,
            organizationName, OrganizationName,
            doctorName, DoctorName,
            startTime, StartTime,
            totalAmount, TotalAmount,
            paidAmount, PaidAmount
        );
        // Compute age from birth_date (format: "YYYY-MM-DD" or empty)
        std::string birthDate = sub.getAgeText();
        if (!birthDate.empty() && birthDate.length() >= 4) {
            int birthYear = std::stoi(birthDate.substr(0, 4));
            int age = currentYear - birthYear;
            dto->ageText = std::to_string(age) + " years";
        }
        // Fallback gender text: dictionary uses numeric codes (0/1) but
        // patient data stores string enums (MALE/FEMALE). Map known codes here.
        std::string genderText = dto->genderEnumText.getValue("");
        if (genderText.empty()) {
            if (dto->genderCode == "MALE") dto->genderEnumText = "Male";
            else if (dto->genderCode == "FEMALE") dto->genderEnumText = "Female";
        }
        // Fallback charge item status summary: maps queue label to display text
        // Dictionary codes (numeric) don't match business enums (REFUNDING/REFUNDED)
        if (dto->chargeItemStatusSummary.getValue("").empty()) {
            std::string qLabel = sub.getQueueLabelCode();
            if (qLabel == "PENDING") dto->chargeItemStatusSummary = "Refunding";
            else if (qLabel == "SETTLED") dto->chargeItemStatusSummary = "Refunded";
        }
        pages->addData(dto);
    }
    return pages;
}
