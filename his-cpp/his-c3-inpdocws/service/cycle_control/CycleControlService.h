#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:23:49

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
#ifndef _CYCLE_CONTROL_SERVICE_H_
#define _CYCLE_CONTROL_SERVICE_H_

#include "domain/vo/cycle_control/PatientQueuePageVO.h"
#include "domain/vo/cycle_control/DischargeOrderVO.h"
#include "domain/query/cycle_control/PatientQueueQuery.h"
#include "domain/dto/cycle_control/DischargeOrderDTO.h"

/**
 * 住院医生站周期控制业务逻辑服务。
 */
class CycleControlService
{
public:
	PatientQueuePageDTO::Wrapper queryPatientQueue(const PatientQueueQuery::Wrapper& query);
	DischargeOrderDetailDTO::Wrapper createDischargeOrder(const CreateDischargeOrderDTO::Wrapper& dto);
};

#endif // !_CYCLE_CONTROL_SERVICE_H_
