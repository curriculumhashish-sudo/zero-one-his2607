#include "stdafx.h"
#include "Prescription3Controller.h"

namespace
{
Prescription3OrderItemDTO::Wrapper exampleOrder(
    const std::string& id,
    const std::string& type)
{
    auto item = Prescription3OrderItemDTO::createShared();
    item->id = id;
    item->type = type;
    return item;
}

Prescription3DeleteJsonVO::Wrapper makeDeleteResponse(
    bool success,
    int affectedCount,
    const std::string& message,
    int code = 10000)
{
    auto vo = Prescription3DeleteJsonVO::createShared();
    vo->data = Prescription3DeleteResultDTO::createShared();
    vo->data->success = success;
    vo->data->affectedCount = affectedCount;
    vo->data->message = message;
    vo->setStatus(ResultStatus(message, code));
    return vo;
}

Prescription3MergeJsonVO::Wrapper makeMergeResponse(
    bool success,
    int affectedCount,
    const std::string& groupId,
    const std::string& message,
    int code = 10000)
{
    auto vo = Prescription3MergeJsonVO::createShared();
    vo->data = Prescription3MergeResultDTO::createShared();
    vo->data->success = success;
    vo->data->affectedCount = affectedCount;
    vo->data->groupId = groupId;
    vo->data->message = message;
    vo->setStatus(ResultStatus(message, code));
    return vo;
}

Prescription3SplitJsonVO::Wrapper makeSplitResponse(
    bool success,
    int affectedCount,
    const std::string& message,
    int code = 10000)
{
    auto vo = Prescription3SplitJsonVO::createShared();
    vo->data = Prescription3SplitResultDTO::createShared();
    vo->data->success = success;
    vo->data->affectedCount = affectedCount;
    vo->data->message = message;
    vo->setStatus(ResultStatus(message, code));
    return vo;
}
}

void Prescription3Controller::addBatchDeleteSwaggerExamples(
    const std::shared_ptr<EndpointInfo>& info)
{
    auto request = Prescription3BatchDeleteDTO::createShared();
    request->orders = {
        exampleOrder("331100000000000001", "MEDICATION"),
        exampleOrder("331100000000000002", "SERVICE")
    };
    info->body.description =
        ZH_WORDS_GETTER("prescription3.batchDelete.description");
    info->body.addExample("default", request);
    info->responses[Status::CODE_200].addExample(
        "success",
        makeDeleteResponse(
            true, 3, "Orders deleted successfully"));
    info->responses[Status::CODE_200].addExample(
        "invalidStatus",
        makeDeleteResponse(
            false,
            0,
            "Only DRAFT orders can be changed",
            PRESCRIPTION3_INVALID_STATUS));
}

void Prescription3Controller::addMergeSwaggerExamples(
    const std::shared_ptr<EndpointInfo>& info)
{
    auto request = Prescription3MergeGroupDTO::createShared();
    request->orders = {
        exampleOrder("331100000000000001", "MEDICATION"),
        exampleOrder("331100000000000002", "DEVICE")
    };
    info->body.description =
        ZH_WORDS_GETTER("prescription3.mergeGroup.description");
    info->body.addExample("default", request);
    info->responses[Status::CODE_200].addExample(
        "success",
        makeMergeResponse(
            true,
            3,
            "331234567890123456",
            "Orders merged successfully"));
    info->responses[Status::CODE_200].addExample(
        "scopeMismatch",
        makeMergeResponse(
            false,
            0,
            "",
            "Orders must have the same business scope",
            PRESCRIPTION3_SCOPE_MISMATCH));
}

void Prescription3Controller::addSplitSwaggerExamples(
    const std::shared_ptr<EndpointInfo>& info)
{
    auto request = Prescription3SplitGroupDTO::createShared();
    request->orders = {
        exampleOrder("331100000000000001", "MEDICATION")
    };
    info->body.description =
        ZH_WORDS_GETTER("prescription3.splitGroup.description");
    info->body.addExample("default", request);
    info->responses[Status::CODE_200].addExample(
        "success",
        makeSplitResponse(
            true, 3, "Order group split successfully"));
    info->responses[Status::CODE_200].addExample(
        "invalidGroup",
        makeSplitResponse(
            false,
            0,
            "Only grouped orders can be split",
            PRESCRIPTION3_INVALID_GROUP));
}

Prescription3DeleteJsonVO::Wrapper
Prescription3Controller::execBatchDeletePrescription3(
    const Prescription3BatchDeleteDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    try
    {
        Prescription3Service service;
        const auto result = service.batchDelete(dto, payload.getId());
        return makeDeleteResponse(
            true,
            result.affectedRows,
            "Orders deleted successfully");
    }
    catch (const Prescription3BusinessException& exception)
    {
        return makeDeleteResponse(
            false, 0, exception.what(), exception.code());
    }
    catch (const sql::SQLException&)
    {
        return makeDeleteResponse(
            false,
            0,
            "Database operation failed",
            PRESCRIPTION3_DATABASE_ERROR);
    }
    catch (const std::exception&)
    {
        return makeDeleteResponse(
            false, 0, "Server operation failed", 9994);
    }
}

Prescription3MergeJsonVO::Wrapper
Prescription3Controller::execMergePrescription3Group(
    const Prescription3MergeGroupDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    try
    {
        Prescription3Service service;
        const auto result = service.mergeGroup(dto, payload.getId());
        return makeMergeResponse(
            true,
            result.affectedRows,
            result.groupId,
            "Orders merged successfully");
    }
    catch (const Prescription3BusinessException& exception)
    {
        return makeMergeResponse(
            false, 0, "", exception.what(), exception.code());
    }
    catch (const sql::SQLException&)
    {
        return makeMergeResponse(
            false,
            0,
            "",
            "Database operation failed",
            PRESCRIPTION3_DATABASE_ERROR);
    }
    catch (const std::exception&)
    {
        return makeMergeResponse(
            false, 0, "", "Server operation failed", 9994);
    }
}

Prescription3SplitJsonVO::Wrapper
Prescription3Controller::execSplitPrescription3Group(
    const Prescription3SplitGroupDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    try
    {
        Prescription3Service service;
        const auto result = service.splitGroup(dto, payload.getId());
        return makeSplitResponse(
            true,
            result.affectedRows,
            "Order group split successfully");
    }
    catch (const Prescription3BusinessException& exception)
    {
        return makeSplitResponse(
            false, 0, exception.what(), exception.code());
    }
    catch (const sql::SQLException&)
    {
        return makeSplitResponse(
            false,
            0,
            "Database operation failed",
            PRESCRIPTION3_DATABASE_ERROR);
    }
    catch (const std::exception&)
    {
        return makeSplitResponse(
            false, 0, "Server operation failed", 9994);
    }
}
