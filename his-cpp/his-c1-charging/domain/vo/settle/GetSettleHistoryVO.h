#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: ovo
 @Date: 2026/07/20

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
#ifndef _GET_SETTLE_HISTORY_VO_H_
#define _GET_SETTLE_HISTORY_VO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/settle/GetSettleHistoryDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSettleHistoryJsonVO : public JsonVO<GetSettleHistoryPageDTO::Wrapper>
{
	DTO_INIT(GetSettleHistoryJsonVO, JsonVO<GetSettleHistoryPageDTO::Wrapper>);

public:
	GetSettleHistoryJsonVO()
	{
		this->data = GetSettleHistoryPageDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_SETTLE_HISTORY_VO_H_
