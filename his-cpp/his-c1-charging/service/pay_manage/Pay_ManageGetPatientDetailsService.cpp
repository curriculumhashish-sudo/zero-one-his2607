#include "stdafx.h"
#include "Pay_ManageGetPatientDetailsService.h"
#include "dao/pay_manage/Pay_ManageGetPatientDetailsDAO.h"

// Prevent ODBC/MySQL type conflict
#include "NacosClient.h"

Pay_ManageGetPatientDetailsDTO::Wrapper Pay_ManageGetPatientDetailsService::getDetails(
    const Pay_ManageGetPatientDetailsQuery::Wrapper& query)
{
    Pay_ManageGetPatientDetailsDAO dao;
    auto data = dao.selectByBusNo(query);

    auto dto = Pay_ManageGetPatientDetailsDTO::createShared();

    // If no data found, return empty DTO
    if (data.getHospitalizedNumber().empty())
    {
        return dto;
    }

    // Direct mapping fields
    dto->hospitalized_number = data.getHospitalizedNumber().empty() ? oatpp::String() : oatpp::String(data.getHospitalizedNumber());
    dto->name = data.getPatientName().empty() ? oatpp::String() : oatpp::String(data.getPatientName());
    dto->depart = data.getDepartmentName().empty() ? oatpp::String() : oatpp::String(data.getDepartmentName());
    dto->ward = data.getWardName().empty() ? oatpp::String() : oatpp::String(data.getWardName());
    dto->bed_number = data.getBedNumber().empty() ? oatpp::String() : oatpp::String(data.getBedNumber());
    dto->visitStatus = data.getVisitStatus().empty() ? oatpp::String() : oatpp::String(data.getVisitStatus());
    dto->cost_attr = data.getCostNature().empty() ? oatpp::String() : oatpp::String(data.getCostNature());

    // Type conversion: string -> Float64 for balance
    try { dto->rest = std::stod(data.getAccountBalance()); }
    catch (...) { dto->rest = 0.0; }

    return dto;
}
