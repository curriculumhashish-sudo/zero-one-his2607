#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

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
#ifndef _SUMMARY_SLIP_VO_
#define _SUMMARY_SLIP_VO_

#include "../../GlobalInclude.h"
#include "../../dto/medicationcollection/SummarySlipDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SubmitSummaryJsonVO : public JsonVO<SubmitSummaryResponseDTO::Wrapper>
{
	DTO_INIT(SubmitSummaryJsonVO, JsonVO<SubmitSummaryResponseDTO::Wrapper>);

public:
	SubmitSummaryJsonVO()
	{
		this->data = SubmitSummaryResponseDTO::createShared();
	}
};

class CancelSummaryJsonVO : public JsonVO<CancelSummaryResponseDTO::Wrapper>
{
	DTO_INIT(CancelSummaryJsonVO, JsonVO<CancelSummaryResponseDTO::Wrapper>);

public:
	CancelSummaryJsonVO()
	{
		this->data = CancelSummaryResponseDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUMMARY_SLIP_VO_
