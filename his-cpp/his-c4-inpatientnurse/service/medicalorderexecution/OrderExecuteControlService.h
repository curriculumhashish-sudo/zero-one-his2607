/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#pragma once

#ifndef _ORDER_EXECUTE_CONTROL_SERVICE_
#define _ORDER_EXECUTE_CONTROL_SERVICE_

#include <string>
#include <unordered_set>

#include "domain/dto/PayloadDTO.h"
#include "domain/dto/medicalorderexecution/OrderExecuteDTO.h"
#include "domain/dto/medicalorderexecution/MedicalOrderExecutionDTO.h"

class OrderExecuteControlService
{
public:
	std::string skipOrderExecution(
		const oatpp::List<OrderNotExecuteDTO::Wrapper>& dtolist,
		const PayloadDTO& payload);

	CancelExecutionResultDTO::Wrapper cancelOrderExecution(
		const CancelExecutionRequestDTO::Wrapper& dto,
		const PayloadDTO& payload);
	BatchOrderExecutionResponseDTO::Wrapper skipBatch(
		const BatchSkipOrderRequestDTO::Wrapper& dto,
		const PayloadDTO& payload);
	BatchOrderExecutionResponseDTO::Wrapper cancelBatch(
		const BatchCancelExecutionRequestDTO::Wrapper& dto,
		const PayloadDTO& payload);

	std::unordered_set<std::string> listOperableEncounterIds(const PayloadDTO& payload) const;

	bool canOperateEncounter(
		const std::unordered_set<std::string>& operableEncounterIds,
		const std::string& encounterId) const;

private:
	std::string buildNotExecuteRequestKey(const OrderNotExecuteDTO::Wrapper& dto) const;
};

#endif // !_ORDER_EXECUTE_CONTROL_SERVICE_
