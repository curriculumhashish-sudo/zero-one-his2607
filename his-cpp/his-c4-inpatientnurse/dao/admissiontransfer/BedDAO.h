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
#ifndef _BED_DAO_
#define _BED_DAO_
#include "BaseDAO.h"
#include "../../domain/do/common/LocationDO.h"
#include "../../domain/do/common/BedReservationDO.h"
#include "../../domain/query/admissiontransfer/BedQuery.h"

// Bed DAO: query free beds + reserve(assign) a bed
class BedDAO : public BaseDAO
{
private:
    // Build the shared WHERE clause (free-bed condition)
    inline std::string queryConditionBuilder(const BedQuery::Wrapper& query, SqlParams& params);
public:
    // Count free beds
    uint64_t countFree(const BedQuery::Wrapper& query);
    // Query free beds with pagination
    std::list<adm_locationDO> selectFreeWithPage(const BedQuery::Wrapper& query);
    // Insert a reservation record (assign). Returns affected rows.
    int reserve(const bkg_bed_reservation_extDO& data);
};

#endif // !_BED_DAO_
