#ifndef _PRACTITIONER_OPTION_CONTROLLER_H_
#define _PRACTITIONER_OPTION_CONTROLLER_H_

#include "ApiHelper.h"
#include "Macros.h"

#include "domain/vo/dept/PractitionerOptionVO.h"
#include "service/dept/PractitionerOptionService.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("dept.tag")

/**
 * Provides the doctor options used by the department owner selector.
 * The result is restricted by the authenticated user's tenant and the
 * ROLE_DOCTOR assignment relationship.
 */
class PractitionerOptionController
    : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PractitionerOptionController);

public:
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("dept.practitionerOption.endpoints.list"),
        listDoctorOptions,
        PractitionerOptionJsonVO::Wrapper,
        API_TAG,
        info->addResponse<PractitionerOptionErrorVO::Wrapper>(
            Status::CODE_403,
            "application/json"
        );
        info->addResponse<PractitionerOptionErrorVO::Wrapper>(
            Status::CODE_500,
            "application/json"
        );
    );

    ENDPOINT(
        API_M_GET,
        "/dept/practitioner/options",
        listDoctorOptions,
        API_HANDLER_AUTH_PARAME
    )
    {
        return executeListDoctorOptions(authObject->getPayload());
    }

private:
    std::shared_ptr<OutgoingResponse> executeListDoctorOptions(
        const PayloadDTO& payload
    );

    std::shared_ptr<OutgoingResponse> createErrorResponse(
        const Status& status,
        int code,
        const std::string& messageKey
    );
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_PRACTITIONER_OPTION_CONTROLLER_H_
