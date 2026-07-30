/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#pragma once
#ifndef _BED_SERVICE_
#define _BED_SERVICE_
#include "domain/vo/admissiontransfer/BedVO.h"
#include "domain/query/admissiontransfer/BedQuery.h"
#include "domain/dto/admissiontransfer/BedAssignDTO.h"

// Bed service: query free beds + assign a bed
class BedService
{
public:
    // Query free beds with pagination
    BedPageDTO::Wrapper listFree(const BedQuery::Wrapper& query);
    // Assign(reserve) a bed. Returns the new record id, or "" on failure/duplicate.
    std::string assignBed(const BedAssignAddDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_BED_SERVICE_
