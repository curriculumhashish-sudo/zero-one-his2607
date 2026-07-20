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

PatientQueuePageVO::Wrapper CycleControlController::execQueryPatientQueue(const PatientQueueQuery::Wrapper& query)
{
	auto jvo = PatientQueuePageVO::createShared();
	jvo->success(PatientQueuePageDTO::createShared());
	return jvo;
}

DischargeAdviceVO::Wrapper CycleControlController::execCreateDischargeAdvice(const CreateDischargeAdviceDTO::Wrapper& dto)
{
	auto jvo = DischargeAdviceVO::createShared();
	jvo->success(DischargeAdviceDetailDTO::createShared());
	return jvo;
}
