#pragma once

#ifndef _ORDER_EXECUTE_CONTROL_SERVICE_
#define _ORDER_EXECUTE_CONTROL_SERVICE_

#include <string>
#include <unordered_set>

#include "domain/dto/PayloadDTO.h"
#include "domain/dto/medicalorderexecution/OrderExecuteDTO.h"

class OrderExecuteControlService
{
public:
	std::string skipOrderExecution(
		const oatpp::List<OrderNotExecuteDTO::Wrapper>& dtolist,
		const PayloadDTO& payload);

	CancelExecutionResultDTO::Wrapper cancelOrderExecution(
		const CancelExecutionRequestDTO::Wrapper& dto,
		const PayloadDTO& payload);

	std::unordered_set<std::string> listOperableEncounterIds(const PayloadDTO& payload) const;

	bool canOperateEncounter(
		const std::unordered_set<std::string>& operableEncounterIds,
		const std::string& encounterId) const;

private:
	std::string buildNotExecuteRequestKey(const OrderNotExecuteDTO::Wrapper& dto) const;
};

#endif // !_ORDER_EXECUTE_CONTROL_SERVICE_
