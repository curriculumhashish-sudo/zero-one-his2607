#include "stdafx.h"
#include "Pay_ManageGetPatientQueueService.h"
#include "dao/pay_manage/Pay_ManageGetPatientQueueDAO.h"

// Prevent ODBC/MySQL type conflict
#include "NacosClient.h"

#include <ctime>

// ============================================================================
// Helper methods: enum translation, age calculation
// ============================================================================

std::string Pay_ManageGetPatientQueueService::translateGender(const std::string& code)
{
    if (code == "MALE")   return ZH_WORDS_GETTER("byj.charging.pay_manage.male");
    if (code == "FEMALE") return ZH_WORDS_GETTER("byj.charging.pay_manage.female");
    return code;
}

std::string Pay_ManageGetPatientQueueService::computeAge(const std::string& birthDate)
{
    if (birthDate.empty() || birthDate.length() < 4) return "";

    int birthYear = 0;
    try { birthYear = std::stoi(birthDate.substr(0, 4)); }
    catch (...) { return ""; }

    time_t now = time(nullptr);
    struct tm* tmNow = localtime(&now);
    int currentYear = tmNow->tm_year + 1900;
    int age = currentYear - birthYear;
    if (age < 0) return "";

    return std::to_string(age);
}

// ============================================================================
// Main method: query, convert, assemble
// ============================================================================

Pay_ManageGetPatientQueuePageDTO::Wrapper Pay_ManageGetPatientQueueService::listAll(
    const Pay_ManageGetPatientQueueQuery::Wrapper& query)
{
    // Step 1: Query total count matching conditions
    Pay_ManageGetPatientQueueDAO dao;
    uint64_t totalCount = dao.count(query);

    // Step 2: Paginated query for current page data
    auto doList = dao.selectWithPage(query);

    // Step 3: Initialize page DTO and fill pagination metadata
    auto pageDTO = Pay_ManageGetPatientQueuePageDTO::createShared();
    pageDTO->pageIndex = query->pageIndex.getValue(1);
    pageDTO->pageSize  = query->pageSize.getValue(10);
    pageDTO->total = static_cast<int64_t>(totalCount);
    pageDTO->calcPages();
    pageDTO->rows = oatpp::List<Pay_ManageGetPatientQueueDTO::Wrapper>::createShared();

    if (doList.empty())
        return pageDTO;

    // Step 4: Convert each DO to DTO and translate/compute fields
    for (auto& dobj : doList)
    {
        auto dto = Pay_ManageGetPatientQueueDTO::createShared();

        // Direct mapping fields
        dto->name = dobj.getPatientName().empty() ? oatpp::String() : oatpp::String(dobj.getPatientName());
        dto->sex = translateGender(dobj.getGenderCode());
        uint64_t ageNum = 0;
        if (!computeAge(dobj.getBirthDate()).empty())
            ageNum = std::stoull(computeAge(dobj.getBirthDate()));
        dto->age = oatpp::UInt64(ageNum);
        dto->hospitalized_number = dobj.getHospitalizedNumber().empty() ? oatpp::String() : oatpp::String(dobj.getHospitalizedNumber());
        dto->hospitalized_time = dobj.getAdmissionTime().empty() ? oatpp::String() : oatpp::String(dobj.getAdmissionTime());
        dto->depart = dobj.getDepartmentName().empty() ? oatpp::String() : oatpp::String(dobj.getDepartmentName());
        dto->bed_number = dobj.getBedNumber().empty() ? oatpp::String() : oatpp::String(dobj.getBedNumber());
        dto->cost_attr = dobj.getCostNature().empty() ? oatpp::String() : oatpp::String(dobj.getCostNature());

        // Type conversion: string -> Float64 for balance
        try { dto->rest = std::stod(dobj.getAccountBalance()); }
        catch (...) { dto->rest = 0.0; }

        // Add to current page data list
        pageDTO->addData(dto);
    }

    return pageDTO;
}
