#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/26 00:00:00

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
#ifndef _NURSING_DAO_
#define _NURSING_DAO_
#include "BaseDAO.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"
#include "domain/query/nursing/NursingRecordQuery.h"

// Lightweight data struct for paper-header cross-table query results.
// DAO returns this; Service maps it to NursingRecordPaperHeaderDTO.
struct PaperHeaderData
{
    std::string inpatientNo;
    std::string patientName;
    std::string gender;
    int age = 0;
    std::string admitAt;
    std::string dischargeAt;
    std::string wardName;
    std::string bedNo;
};

// Nursing record DAO — queries dia_vital_sign table (query-only per skill convention)
class NursingDAO : public BaseDAO
{
public:
    // Build WHERE clause from query parameters
    std::string queryConditionBuilder(const NursingRecordQuery::Wrapper& query, SqlParams& params);
    // Count nursing records matching filters
    uint64_t count(const NursingRecordQuery::Wrapper& query);
    // Query vital sign records by condition (ordered by recorded_at)
    std::list<VitalSignDO> selectByCondition(const NursingRecordQuery::Wrapper& query);
    // Query patient/encounter info for paper header; returns raw data struct
    std::shared_ptr<PaperHeaderData> selectPaperHeader(const std::string& encounterId, const std::string& patientId);
    // Batch query detail rows by parent vital-sign IDs
    std::list<VitalSignDetailDO> selectByVitalSignIds(const std::list<std::string>& ids);
};

#endif // !_NURSING_DAO_
