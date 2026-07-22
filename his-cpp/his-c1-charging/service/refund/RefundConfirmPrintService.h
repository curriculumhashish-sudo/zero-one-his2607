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
#ifndef _CONFIRM_PRINT_SERVICE_
#define _CONFIRM_PRINT_SERVICE_

#include "domain/vo/refund/ConfirmPrintVO.h"
#include "domain/query/refund/ConfirmPrintQuery.h"
#include "domain/dto/refund/ConfirmPrintDTO.h"

class ConfirmPrintService
{
public:
    // Confirm the print job result
    ConfirmPrintDTO::Wrapper confirm(const ConfirmPrintQuery::Wrapper& query);
};

#endif // !_CONFIRM_PRINT_SERVICE_
