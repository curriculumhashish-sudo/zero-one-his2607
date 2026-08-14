/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 10:58:42

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
#include "stdafx.h"
#include "CycleControlController.h"
#include "service/cycle_control/CycleControlService.h"

PatientQueuePageVO::Wrapper CycleControlController::execQueryPatientQueue(const PatientQueueQuery::Wrapper& query)
{
	auto jvo = PatientQueuePageVO::createShared();

	// 分页参数校验
	if (!query->pageIndex || query->pageIndex.getValue(1) < 1
		|| !query->pageSize || query->pageSize.getValue(10) < 1 || query->pageSize.getValue(10) > 100)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	auto data = CycleControlService().queryPatientQueue(query);
	jvo->success(data);
	return jvo;
}

DischargeOrderVO::Wrapper CycleControlController::execCreateDischargeOrder(const CreateDischargeOrderDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = DischargeOrderVO::createShared();

	string errmsg = dto->validate();
	if (!errmsg.empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	dto->setPayload(&payload);
	auto data = CycleControlService().createDischargeOrder(dto);
	if (data) {
		jvo->success(data);
	}
	else {
		jvo->fail(nullptr);
	}
	return jvo;
}
