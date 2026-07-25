#pragma once

#ifndef _PRESCRIPTION2CONTROLLER_H_
#define _PRESCRIPTION2CONTROLLER_H_

#include "domain/dto/prescription2/PrintApplicationFormDTO.h"
#include "domain/dto/prescription2/SignMedicalAdviceDTO.h"
#include "domain/dto/prescription2/WithdrawMedicalAdviceDTO.h"
#include "domain/vo/prescription2/MedicalAdviceVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medical-advice.tags.tag")

class Prescription2Controller : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(Prescription2Controller);

public: // 定义接口端点
	// 定义批量签署医嘱接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medical-advice.sign.summary"),
		signMedicalAdvice, SignMedicalAdviceJsonVO::Wrapper, API_TAG);
	// 定义批量签署医嘱接口处理器
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/medical-advice/sign", signMedicalAdvice,
		BODY_DTO(SignMedicalAdviceDTO::Wrapper, dto),
		execSignMedicalAdvice(dto, authObject->getPayload()));

	// 定义批量撤回医嘱接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medical-advice.withdraw.summary"),
		withdrawMedicalAdvice, WithdrawMedicalAdviceJsonVO::Wrapper, API_TAG);
	// 定义批量撤回医嘱接口处理器
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/medical-advice/withdraw", withdrawMedicalAdvice,
		BODY_DTO(WithdrawMedicalAdviceDTO::Wrapper, dto),
		execWithdrawMedicalAdvice(dto, authObject->getPayload()));

	// 定义申请单打印接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("medical-advice.print-application-form.summary"),
		printApplicationForm, PrintApplicationFormJsonVO::Wrapper, API_TAG);
	// 定义申请单打印接口处理器
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/medical-advice/print-application-form", printApplicationForm,
		BODY_DTO(PrintApplicationFormDTO::Wrapper, dto),
		execPrintApplicationForm(dto, authObject->getPayload()));

private: // 定义接口执行函数
	// 声明批量签署医嘱执行函数
	SignMedicalAdviceJsonVO::Wrapper execSignMedicalAdvice(
		const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
	// 声明批量撤回医嘱执行函数
	WithdrawMedicalAdviceJsonVO::Wrapper execWithdrawMedicalAdvice(
		const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
	// 声明申请单打印执行函数
	PrintApplicationFormJsonVO::Wrapper execPrintApplicationForm(
		const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PRESCRIPTION2CONTROLLER_H_
