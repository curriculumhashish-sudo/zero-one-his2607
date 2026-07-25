#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/medicalorderexecution/OrderExecuteDTO.h"
#include "domain/vo/medicalorderexecution/OrderExecuteVO.h"

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
			OrderNotExecuteDTO::createShared()};
		info->body.addExample("default", example);
		info->responses[Status::CODE_200].addExample(
			"success", StringJsonVO::createShared());
	);
	// Skip medical-order execution.
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/advice/notexecute",
		skipOrderExecution,
		BODY_DTO(List<OrderNotExecuteDTO::Wrapper>, dto),
		execSkipOrderExecution(dto, authObject->getPayload())
	);


	// Swagger definition for cancelling medical-order execution.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical_advice.cancelexecute.summary"),
		cancelOrderExecution,
		CancelExecutionJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", CancelExecutionRequestDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", CancelExecutionJsonVO::createShared());
	);
	// Cancel medical-order execution.
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/advice/cancelexecute",
		cancelOrderExecution,
		BODY_DTO(CancelExecutionRequestDTO::Wrapper, dto),
		execCancelOrderExecution(dto, authObject->getPayload())
	);
private:
	StringJsonVO::Wrapper execSkipOrderExecution(
		const List<OrderNotExecuteDTO::Wrapper>&, 
		const PayloadDTO&);

	CancelExecutionJsonVO::Wrapper execCancelOrderExecution(
		const CancelExecutionRequestDTO::Wrapper&,
		const PayloadDTO&);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
