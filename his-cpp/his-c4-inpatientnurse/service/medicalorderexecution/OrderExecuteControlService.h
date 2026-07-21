#pragma once

#ifndef _ORDER_EXECUTE_CONTROL_SERVICE_
#define _ORDER_EXECUTE_CONTROL_SERVICE_

#include <string>

#include "domain/dto/PayloadDTO.h"
#include "domain/dto/medicalorderexecution/OrderExecuteDTO.h"

class OrderExecuteControlService
{
public:
	std::string skipOrderExecution(
		const oatpp::List<OrderNotExecuteDTO::Wrapper>& dtolist,
		const PayloadDTO& payload);

private:
	std::string buildNotExecuteRequestKey(const OrderNotExecuteDTO::Wrapper& dto) const;
};

#endif // !_ORDER_EXECUTE_CONTROL_SERVICE_
