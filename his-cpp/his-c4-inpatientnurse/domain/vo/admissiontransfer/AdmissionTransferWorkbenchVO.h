#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/08/06 00:00:00

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
#ifndef _ADMISSION_TRANSFER_WORKBENCH_VO_
#define _ADMISSION_TRANSFER_WORKBENCH_VO_

#include "../../GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"
#include "../../dto/admissiontransfer/AdmissionTransferWorkbenchDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class BedBoardJsonVO : public JsonVO<BedBoardDTO::Wrapper>
{
	DTO_INIT(BedBoardJsonVO, JsonVO<BedBoardDTO::Wrapper>);

public:
	BedBoardJsonVO()
	{
		data = BedBoardDTO::createShared();
	}
};

class BedOperationJsonVO : public JsonVO<BedOperationResponseDTO::Wrapper>
{
	DTO_INIT(BedOperationJsonVO, JsonVO<BedOperationResponseDTO::Wrapper>);

public:
	BedOperationJsonVO()
	{
		data = BedOperationResponseDTO::createShared();
	}
};

class RecallInpatientDischargeJsonVO : public JsonVO<RecallInpatientDischargeResponseDTO::Wrapper>
{
	DTO_INIT(RecallInpatientDischargeJsonVO, JsonVO<RecallInpatientDischargeResponseDTO::Wrapper>);

public:
	RecallInpatientDischargeJsonVO()
	{
		data = RecallInpatientDischargeResponseDTO::createShared();
	}
};

class DischargePendingListJsonVO : public ListJsonVO<DischargePendingItemDTO::Wrapper>
{
	DTO_INIT(DischargePendingListJsonVO, ListJsonVO<DischargePendingItemDTO::Wrapper>);

public:
	DischargePendingListJsonVO()
	{
		data = List<DischargePendingItemDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ADMISSION_TRANSFER_WORKBENCH_VO_
