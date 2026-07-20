#pragma once

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/medicalorderexecution/OrderExecuteDTO.h"
#include "domain/vo/medicalorderexecution/OrderExecuteVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medical_advice.tag")

class OrderExecuteControlController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(OrderExecuteControlController);
public:
	/*不执行医嘱接口文档声明*/
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical_advice.notexecute.summary"),
		skipOrderExecution,
		StringJsonVO::Wrapper,
		API_TAG
	);
	/*定义不执行医嘱接口*/
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/advice/notexecute",
		skipOrderExecution,
		BODY_DTO(List<OrderNotExecuteDTO::Wrapper>, dto),
		execSkipOrderExecution(dto, authObject->getPayload())
	);


	/*取消执行医嘱接口文档声明*/
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medical_advice.cancelexecute.summary"),
		cancelOrderExecution,
		CancelExecutionJsonVO::Wrapper,
		API_TAG
	);
	/*定义取消执行医嘱接口*/
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
