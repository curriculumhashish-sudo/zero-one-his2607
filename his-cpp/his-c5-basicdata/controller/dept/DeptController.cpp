#include "stdafx.h"
#include "DeptController.h"

#include "domain/dept/DeptStatus.h"
#include "service/dept/DeptService.h"

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

std::shared_ptr<DeptController::OutgoingResponse> DeptController::execAddDept(
    const DeptAddDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    // 定义返回数据对象
    auto vo = DeptAddJsonVO::createShared();
    // 参数校验
    if (!dto)
    {
        vo->init(nullptr, DeptStatus::invalid("request body is required."));
        return createDtoResponse(Status::CODE_400, vo);
    }

    const std::string error = dto->validate();
    if (!error.empty())
    {
        vo->init(nullptr, DeptStatus::invalid(error));
        return createDtoResponse(Status::CODE_400, vo);
    }

    // 执行数据新增
    dto->setPayload(&payload);
    //auto result = DeptService().add(dto);
    //// 返回结果
    //vo->init(
    //    result.success ? oatpp::String(result.id.c_str()) : oatpp::String(nullptr),
    //    result.status);
    //return createDtoResponse(toHttpStatus(result.httpStatus), vo);
    return createDtoResponse(toHttpStatus(200), vo);
}

std::shared_ptr<DeptController::OutgoingResponse> DeptController::execModifyDept(
    const DeptModifyDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    // 定义返回数据对象
    auto vo = DeptModifyJsonVO::createShared();
    // 参数校验
    if (!dto)
    {
        vo->init(nullptr, DeptStatus::invalid("request body is required."));
        return createDtoResponse(Status::CODE_400, vo);
    }

    const std::string error = dto->validate();
    if (!error.empty())
    {
        vo->init(nullptr, DeptStatus::invalid(error));
        return createDtoResponse(Status::CODE_400, vo);
    }

    // 执行数据修改
    dto->setPayload(&payload);
    //auto result = DeptService().modify(dto);
    //// 返回结果
    //vo->init(
    //    result.success ? oatpp::String(result.id.c_str()) : oatpp::String(nullptr),
    //    result.status);
    //return createDtoResponse(toHttpStatus(result.httpStatus), vo);
    return createDtoResponse(toHttpStatus(200), vo);
}

std::shared_ptr<DeptController::OutgoingResponse> DeptController::execRemoveDept(
    const DeptDeleteQuery::Wrapper& query,
    const PayloadDTO& payload)
{
    // 定义返回数据对象
    auto vo = DeptDeleteJsonVO::createShared();
    // 参数校验
    if (!query)
    {
        vo->init(nullptr, DeptStatus::invalid("request body is required."));
        return createDtoResponse(Status::CODE_400, vo);
    }

    const std::string error = query->validate();
    if (!error.empty())
    {
        vo->init(nullptr, DeptStatus::invalid(error));
        return createDtoResponse(Status::CODE_400, vo);
    }

    // 执行数据删除
    query->setPayload(&payload);
    //auto result = DeptService().remove(query);
    //// 返回结果
    //vo->init(
    //    result.success ? oatpp::String(result.id.c_str()) : oatpp::String(nullptr),
    //    result.status);
    //return createDtoResponse(toHttpStatus(result.httpStatus), vo);
    return createDtoResponse(toHttpStatus(200), vo);
}
