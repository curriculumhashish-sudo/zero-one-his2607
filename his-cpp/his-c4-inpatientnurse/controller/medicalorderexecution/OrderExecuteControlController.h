#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/medicalorderexecution/OrderExecuteDTO.h"
#include "domain/vo/medicalorderexecution/OrderExecuteVO.h"
#include "domain/vo/medicalorderexecution/MedicalOrderVO.h"
#include "controller/RequestValidation.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medical-order-execution.tag")

class OrderExecuteControlController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(OrderExecuteControlController);
public:
	// Swagger definition for skipping medical-order execution.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical_advice.notexecute.summary"),
		skipOrderExecution,
		StringJsonVO::Wrapper,
		API_TAG,
		oatpp::List<OrderNotExecuteDTO::Wrapper> example = {
			OrderNotExecuteDTO::createExample()};
		info->body.addExample("default", example);
		info->responses[Status::CODE_200].addExample(
			"success", StringJsonVO::createShared());
	);
	// Skip medical-order execution.
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/advice/notexecute",
		skipOrderExecution,
		C4_REQUIRED_BODY_DTO(List<OrderNotExecuteDTO::Wrapper>, dto),
		execSkipOrderExecution(dto, authObject->getPayload())
	);


	// Swagger definition for cancelling medical-order execution.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical_advice.cancelexecute.summary"),
		cancelOrderExecution,
		CancelExecutionJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", CancelExecutionRequestDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", CancelExecutionJsonVO::createShared());
	);
	// Cancel medical-order execution.
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/advice/cancelexecute",
		cancelOrderExecution,
		C4_REQUIRED_BODY_DTO(CancelExecutionRequestDTO::Wrapper, dto),
		execCancelOrderExecution(dto, authObject->getPayload())
	);

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical-order-execution.workbench-skip.summary"),
		skipOrderExecutionBatch,
		BatchOrderExecutionJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", BatchSkipOrderRequestDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", BatchOrderExecutionJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/nurse-station/order-executions/skip",
		skipOrderExecutionBatch,
		C4_REQUIRED_BODY_DTO(BatchSkipOrderRequestDTO::Wrapper, dto),
		execSkipOrderExecutionBatch(dto, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical-order-execution.workbench-cancel.summary"),
		cancelOrderExecutionBatch,
		BatchOrderExecutionJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", BatchCancelExecutionRequestDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", BatchOrderExecutionJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/nurse-station/order-executions/cancel",
		cancelOrderExecutionBatch,
		C4_REQUIRED_BODY_DTO(BatchCancelExecutionRequestDTO::Wrapper, dto),
		execCancelOrderExecutionBatch(dto, authObject->getPayload()));
private:
	StringJsonVO::Wrapper execSkipOrderExecution(
		const List<OrderNotExecuteDTO::Wrapper>&, 
		const PayloadDTO&);

	CancelExecutionJsonVO::Wrapper execCancelOrderExecution(
		const CancelExecutionRequestDTO::Wrapper&,
		const PayloadDTO&);
	BatchOrderExecutionJsonVO::Wrapper execSkipOrderExecutionBatch(
		const BatchSkipOrderRequestDTO::Wrapper&, const PayloadDTO&);
	BatchOrderExecutionJsonVO::Wrapper execCancelOrderExecutionBatch(
		const BatchCancelExecutionRequestDTO::Wrapper&, const PayloadDTO&);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
