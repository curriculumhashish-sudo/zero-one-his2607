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
#ifndef _MEDICATION_COLLECTION_VO_
#define _MEDICATION_COLLECTION_VO_

#include "../../GlobalInclude.h"
#include "../../dto/medicationcollection/MedicationCollectionDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SummaryOrderItemPageJsonVO : public JsonVO<SummaryOrderItemPageDTO::Wrapper>
{
	DTO_INIT(SummaryOrderItemPageJsonVO, JsonVO<SummaryOrderItemPageDTO::Wrapper>);

public:
	SummaryOrderItemPageJsonVO()
	{
		this->data = SummaryOrderItemPageDTO::createShared();
	}
};

class SummaryMedicationJsonVO : public JsonVO<SummaryMedicationResultDTO::Wrapper>
{
	DTO_INIT(SummaryMedicationJsonVO, JsonVO<SummaryMedicationResultDTO::Wrapper>);

public:
	SummaryMedicationJsonVO()
	{
		this->data = SummaryMedicationResultDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_COLLECTION_VO_
