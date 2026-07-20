#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/21 00:00:00

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
#ifndef _ADMISSION_TRANSFER_VO_
#define _ADMISSION_TRANSFER_VO_

#include "../../GlobalInclude.h"
#include "../../dto/admissiontransfer/DischargeDTO.h"
#include "../../dto/admissiontransfer/PatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientStatisticsJsonVO : public JsonVO<PatientStatisticsDTO::Wrapper>
{
	DTO_INIT(PatientStatisticsJsonVO, JsonVO<PatientStatisticsDTO::Wrapper>);

public:
	PatientStatisticsJsonVO()
	{
		this->data = PatientStatisticsDTO::createShared();
	}
};

class PatientQueuePageJsonVO : public JsonVO<PatientQueuePageDTO::Wrapper>
{
	DTO_INIT(PatientQueuePageJsonVO, JsonVO<PatientQueuePageDTO::Wrapper>);

public:
	PatientQueuePageJsonVO()
	{
		this->data = PatientQueuePageDTO::createShared();
	}
};

class DischargeJsonVO : public JsonVO<DischargeResultDTO::Wrapper>
{
	DTO_INIT(DischargeJsonVO, JsonVO<DischargeResultDTO::Wrapper>);

public:
	DischargeJsonVO()
	{
		this->data = DischargeResultDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ADMISSION_TRANSFER_VO_
