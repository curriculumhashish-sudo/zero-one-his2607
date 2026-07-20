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
#ifndef _PREPARE_PRINT_VO_
#define _PREPARE_PRINT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/refund/PreparePrintDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Response wrapper: {code, success, data: PreparePrintDTO, message, ...}
class PreparePrintJsonVO : public JsonVO<PreparePrintDTO::Wrapper> {
    DTO_INIT(PreparePrintJsonVO, JsonVO<PreparePrintDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPARE_PRINT_VO_
