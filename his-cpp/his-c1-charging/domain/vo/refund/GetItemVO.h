#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

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
#ifndef _GET_ITEM_VO_
#define _GET_ITEM_VO_

#include "../../GlobalInclude.h"
#include "../../dto/refund/GetItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 单条响应包装: {code, success, data: GetItemDTO, message, ...}
class GetItemJsonVO : public JsonVO<GetItemDTO::Wrapper> {
    DTO_INIT(GetItemJsonVO, JsonVO<GetItemDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_ITEM_VO_
