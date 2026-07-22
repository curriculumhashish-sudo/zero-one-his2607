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
#ifndef _REFUND_PREPARE_PRINT_VO_
#define _REFUND_PREPARE_PRINT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/refund/RefundPreparePrintDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Response wrapper: {code, success, data: RefundPreparePrintDTO, message, ...}
class RefundPreparePrintJsonVO : public JsonVO<RefundPreparePrintDTO::Wrapper> {
    DTO_INIT(RefundPreparePrintJsonVO, JsonVO<RefundPreparePrintDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_REFUND_PREPARE_PRINT_VO_
