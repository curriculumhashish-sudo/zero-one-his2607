#include "stdafx.h"
#include "Patcontroller.h"

#include "StringUtil.h"
#include "domain/pat/PatientStatus.h"
#include "service/pat/PatService.h"

namespace
{
    const oatpp::web::protocol::http::Status& toHttpStatus(int status)
    {
        switch (status)
        {
        case 400: return oatpp::web::protocol::http::Status::CODE_400;
        case 403: return oatpp::web::protocol::http::Status::CODE_403;
        case 404: return oatpp::web::protocol::http::Status::CODE_404;
        case 409: return oatpp::web::protocol::http::Status::CODE_409;
        case 500: return oatpp::web::protocol::http::Status::CODE_500;
        default: return oatpp::web::protocol::http::Status::CODE_200;
        }
    }
}

PatPageJsonVO::Wrapper PatController::execQueryPatPage(
    const PatPageQuery::Wrapper& query,
    const PayloadDTO& payload)
{
    if (!query->pageIndex || query->pageIndex < 1) query->pageIndex = 1;
    if (!query->pageSize || query->pageSize < 1) query->pageSize = 10;

    auto response = PatPageJsonVO::createShared();
    response->success(PatService().queryPatPage(query, payload));
    response->message = ZH_WORDS_GETTER("pat.page.success");
    return response;
}

PatSelectPageJsonVO::Wrapper PatController::execQuerySelectPatPage(
    const PatSelectPageQuery::Wrapper& query,
    const PayloadDTO& payload)
{
    if (!query->pageIndex || query->pageIndex < 1) query->pageIndex = 1;
    if (!query->pageSize || query->pageSize < 1) query->pageSize = 20;

    auto response = PatSelectPageJsonVO::createShared();
    response->success(PatService().querySelectPatPage(query, payload));
    response->message = ZH_WORDS_GETTER("pat.select.page.success");
    return response;
}

std::shared_ptr<PatController::OutgoingResponse> PatController::execQueryById(
    const String& id,
    const PayloadDTO& payload)
{
    auto response = PatientDetailJsonVO::createShared();
    auto query = PatientDetailQuery::createShared();
    query->id = id;
    const std::string error = query->validate();
    if (!error.empty())
    {
        response->init(nullptr, PatientStatus::invalid(error));
        return createDtoResponse(Status::CODE_400, response);
    }

    query->setPayload(&payload);
    const auto result = PatService().queryById(query);
    response->init(result.success ? result.data : nullptr, result.status);
    return createDtoResponse(toHttpStatus(result.httpStatus), response);
}

std::shared_ptr<PatController::OutgoingResponse> PatController::execAddPatient(
    const PatientAddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = StringJsonVO::createShared();
    if (!dto)
    {
        response->init(nullptr, PatientStatus::invalid("request body is required."));
        return createDtoResponse(Status::CODE_400, response);
    }
    const std::string error = dto->validate();
    if (!error.empty())
    {
        response->init(nullptr, PatientStatus::invalid(error));
        return createDtoResponse(Status::CODE_400, response);
    }

    dto->setPayload(&payload);
    const auto result = PatService().add(dto);
    response->init(
        result.success ? oatpp::String(result.id.c_str()) : oatpp::String(nullptr),
        result.status);
    return createDtoResponse(toHttpStatus(result.httpStatus), response);
}

std::shared_ptr<PatController::OutgoingResponse> PatController::execModifyPatient(
    const PatientUpdateDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto response = StringJsonVO::createShared();
    if (!dto)
    {
        response->init(nullptr, PatientStatus::invalid("request body is required."));
        return createDtoResponse(Status::CODE_400, response);
    }
    const std::string error = dto->validate();
    if (!error.empty())
    {
        response->init(nullptr, PatientStatus::invalid(error));
        return createDtoResponse(Status::CODE_400, response);
    }

    dto->setPayload(&payload);
    const auto result = PatService().modify(dto);
    response->init(
        result.success ? oatpp::String(result.id.c_str()) : oatpp::String(nullptr),
        result.status);
    return createDtoResponse(toHttpStatus(result.httpStatus), response);
}

ListJsonVO<String>::Wrapper PatController::execRemovePat(
    const List<String>& ids,
    const PayloadDTO& payload)
{
    auto response = ListJsonVO<String>::createShared();
    if (!ids || ids->empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    PatService service;
    auto deleted = oatpp::List<String>::createShared();
    for (const auto& id : *ids)
    {
        if (!id) continue;
        std::string value = id.getValue("");
        StringUtil::trim(&value);
        if (value.empty() || value.size() > 64) continue;
        if (service.removePatient(value, payload)) deleted->push_back(id);
    }
    response->success(deleted);
    return response;
}
