#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/medicalorder/SignAdviceDTO.h"
#include "domain/dto/medicalorder/WithdrawAdviceDTO.h"
#include "domain/dto/medicalorder/CancelStopAdviceDTO.h"
#include "domain/vo/medicalorder/MedicalOrderOperationResultVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG_ADVICE ZH_WORDS_GETTER("advice.tag")

class MedicalOrderController : public oatpp::web::server::api::ApiController {
	// 三个接口都必须经过认证，操作人从认证信息中获取。
	API_ACCESS_DECLARE(MedicalOrderController);
public:
    // ==================== 签发医嘱 ====================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("advice.sign.summary"), signAdvice, MedicalOrderOperationResultVO::Wrapper, API_TAG_ADVICE);
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/medical-order/sign", signAdvice,
        BODY_DTO(SignAdviceDTO::Wrapper, dto),
        execSignAdvice(dto, authObject->getPayload()));

    // ==================== 撤回医嘱 ====================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("advice.withdraw.summary"), withdrawAdvice, MedicalOrderOperationResultVO::Wrapper, API_TAG_ADVICE);
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/medical-order/withdraw", withdrawAdvice,
        BODY_DTO(WithdrawAdviceDTO::Wrapper, dto),
        execWithdrawAdvice(dto, authObject->getPayload()));

    // ==================== 取消停嘱 ====================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("advice.cancel-stop.summary"), cancelStopAdvice, MedicalOrderOperationResultVO::Wrapper, API_TAG_ADVICE);
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/medical-order/cancel-stop", cancelStopAdvice,
        BODY_DTO(CancelStopAdviceDTO::Wrapper, dto),
        execCancelStopAdvice(dto, authObject->getPayload()));


private:
    MedicalOrderOperationResultVO::Wrapper execSignAdvice(const SignAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    MedicalOrderOperationResultVO::Wrapper execWithdrawAdvice(const WithdrawAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    MedicalOrderOperationResultVO::Wrapper execCancelStopAdvice(const CancelStopAdviceDTO::Wrapper& dto, const PayloadDTO& payload);

};

#undef API_TAG_ADVICE

#include OATPP_CODEGEN_END(ApiController)
