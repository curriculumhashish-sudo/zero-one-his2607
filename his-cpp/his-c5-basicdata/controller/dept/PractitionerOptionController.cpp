#include "stdafx.h"
#include "PractitionerOptionController.h"

#include "domain/vo/BaseJsonVO.h"

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
PractitionerOptionController::executeListDoctorOptions(
    const PayloadDTO& payload
)
{
    try
    {
        PractitionerOptionService service;
        const auto queryResult = service.getDoctorOptions(payload);

        if (!queryResult.tenantFound)
        {
            return createErrorResponse(
                Status::CODE_403,
                RS_FORBIDDEN.getCode(),
                "dept.practitionerOption.errors.tenantUnavailable"
            );
        }

        auto result = PractitionerOptionJsonVO::createShared();
        auto options =
            oatpp::List<PractitionerOptionDTO::Wrapper>::createShared();

        for (const auto& option : queryResult.options)
        {
            options->push_back(option);
        }

        result->success(options);
        return createDtoResponse(Status::CODE_200, result);
    }
    catch (const std::exception& exception)
    {
        OATPP_LOGE(
            "PractitionerOptionController",
            "Doctor option query failed: %s",
            exception.what()
        );
    }
    catch (...)
    {
        OATPP_LOGE(
            "PractitionerOptionController",
            "Doctor option query failed with an unknown exception"
        );
    }

    return createErrorResponse(
        Status::CODE_500,
        RS_SERVER_ERROR.getCode(),
        "dept.practitionerOption.errors.serverError"
    );
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
PractitionerOptionController::createErrorResponse(
    const Status& status,
    int code,
    const std::string& messageKey
)
{
    auto result = PractitionerOptionErrorVO::createShared();
    result->code = code;
    result->message = ZH_WORDS_GETTER(messageKey);
    result->data = nullptr;

    return createDtoResponse(status, result);
}
