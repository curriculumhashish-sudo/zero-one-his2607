#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/08/01 10:00:00

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
#ifndef _PHARMSERVICE_H_
#define _PHARMSERVICE_H_

#include "domain/query/pharm/PharmQuery.h"
#include "domain/dto/pharm/PharmAddDTO.h"
#include "domain/dto/pharm/PharmModifyDTO.h"
#include "domain/vo/pharm/PharmPageVO.h"
#include "domain/vo/pharm/PharmVO.h"
#include "domain/GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"

/**
 * PharmService - Business logic for pharmacy/warehouse management.
 */
class PharmService
{
public:
    // Paginated query for pharmacy/warehouse list
    PharmPageJsonVO::Wrapper getPage(const PharmQuery::Wrapper& query);
    // Name list for dropdown: id + name, optional form_enum filter
    PharmNameListJsonVO::Wrapper nameList(const oatpp::String& formEnum);
    // Add a new pharmacy/warehouse
    StringJsonVO::Wrapper add(const PharmAddDTO::Wrapper& dto);
    // Modify pharmacy/warehouse info
    StringJsonVO::Wrapper modify(const PharmModifyDTO::Wrapper& dto);
    // Soft-delete pharmacy/warehouse
    StringJsonVO::Wrapper remove(const std::string& id);
    // Update pharmacy/warehouse status_enum
    StringJsonVO::Wrapper updateStatus(const std::string& id, const std::string& statusEnum);
};

#endif // !_PHARMSERVICE_H_
