#pragma once

#ifndef _PRESCRIPTION2CONTROLLER_H_
#define _PRESCRIPTION2CONTROLLER_H_

#include "domain/dto/prescription2/PrintApplicationFormDTO.h"
#include "domain/dto/prescription2/SignMedicalAdviceDTO.h"
#include "domain/dto/prescription2/WithdrawMedicalAdviceDTO.h"
#include "domain/vo/prescription2/MedicalAdviceVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medical-advice.tags.tag")

// Medical advice operation controller.
class Prescription2Controller : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(Prescription2Controller);

public:
    // Sign selected medical advice records in draft status.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medical-advice.sign.summary"),
        signMedicalAdvice, SignMedicalAdviceJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/medical-advice/sign", signMedicalAdvice,
        BODY_DTO(SignMedicalAdviceDTO::Wrapper, dto),
        execSignMedicalAdvice(dto, authObject->getPayload()));

    // Withdraw selected signed medical advice records.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medical-advice.withdraw.summary"),
        withdrawMedicalAdvice, WithdrawMedicalAdviceJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/medical-advice/withdraw", withdrawMedicalAdvice,
        BODY_DTO(WithdrawMedicalAdviceDTO::Wrapper, dto),
        execWithdrawMedicalAdvice(dto, authObject->getPayload()));

    // Increment the print count for a service request.
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medical-advice.print-application-form.summary"),
        printApplicationForm, PrintApplicationFormJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/medical-advice/print-application-form", printApplicationForm,
        BODY_DTO(PrintApplicationFormDTO::Wrapper, dto),
        execPrintApplicationForm(dto, authObject->getPayload()));

private:
    // Validate and execute a medical advice signing request.
    SignMedicalAdviceJsonVO::Wrapper execSignMedicalAdvice(
        const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // Validate and execute a medical advice withdrawal request.
    WithdrawMedicalAdviceJsonVO::Wrapper execWithdrawMedicalAdvice(
        const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // Validate and execute a service request print operation.
    PrintApplicationFormJsonVO::Wrapper execPrintApplicationForm(
        const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PRESCRIPTION2CONTROLLER_H_
