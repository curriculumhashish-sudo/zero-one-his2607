#include "stdafx.h"
#include "Prescription2Controller.h"
#include "service/prescription2/MedicalAdviceService.h"

SignMedicalAdviceJsonVO::Wrapper Prescription2Controller::execSignMedicalAdvice(
    const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto response = SignMedicalAdviceJsonVO::createShared();
    // Validate request fields before starting the signing process.
    if (!dto || !dto->encounterId || dto->encounterId->empty() ||
        !dto->version || dto->version < 0 || !dto->adviceIds || dto->adviceIds->empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    // Delegate the status transition to the service layer.
    const auto signedIds = MedicalAdviceService().signMedicalAdvice(dto, payload);
    if (signedIds.empty())
    {
        response->fail(nullptr);
        return response;
    }

    // Convert signed identifiers to the response payload.
    auto data = oatpp::List<oatpp::String>::createShared();
    for (const auto& id : signedIds)
    {
        data->push_back(id.c_str());
    }
    response->success(data);
    return response;
}

WithdrawMedicalAdviceJsonVO::Wrapper Prescription2Controller::execWithdrawMedicalAdvice(
    const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto response = WithdrawMedicalAdviceJsonVO::createShared();
    // Validate request fields before starting the withdrawal process.
    if (!dto || !dto->encounterId || dto->encounterId->empty() ||
        !dto->version || dto->version < 0 || !dto->adviceIds || dto->adviceIds->empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    // Delegate the status transition to the service layer.
    const auto withdrawnIds = MedicalAdviceService().withdrawMedicalAdvice(dto, payload);
    if (withdrawnIds.empty())
    {
        response->fail(nullptr);
        return response;
    }

    // Convert withdrawn identifiers to the response payload.
    auto data = oatpp::List<oatpp::String>::createShared();
    for (const auto& id : withdrawnIds)
    {
        data->push_back(id.c_str());
    }
    response->success(data);
    return response;
}

PrintApplicationFormJsonVO::Wrapper Prescription2Controller::execPrintApplicationForm(
    const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto response = PrintApplicationFormJsonVO::createShared();
    // A print operation must target one service request.
    if (!dto || !dto->serviceRequestId || dto->serviceRequestId->empty())
    {
        response->init(nullptr, RS_PARAMS_INVALID);
        return response;
    }

    // Delegate the print count update to the service layer.
    const std::string serviceRequestId = MedicalAdviceService().printApplicationForm(dto, payload);
    if (serviceRequestId.empty())
    {
        response->fail(nullptr);
        return response;
    }

    response->success(serviceRequestId.c_str());
    return response;
}
