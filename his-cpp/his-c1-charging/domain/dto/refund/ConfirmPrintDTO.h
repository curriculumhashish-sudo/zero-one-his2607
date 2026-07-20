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
#ifndef _CONFIRM_PRINT_DTO_
#define _CONFIRM_PRINT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// ==================== ConfirmPrint Response DTO ====================
// Response data for confirmPrint endpoint
// Example: {"success": true}
class ConfirmPrintDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmPrintDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD_INFO(success) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.success");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CONFIRM_PRINT_DTO_
