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


class NursingDAO : public BaseDAO
{
public:

    std::string queryConditionBuilder(const NursingRecordQuery::Wrapper& query, SqlParams& params);

    uint64_t count(const NursingRecordQuery::Wrapper& query);

    std::list<VitalSignDO> selectByCondition(const NursingRecordQuery::Wrapper& query);

    std::shared_ptr<PaperHeaderData> selectPaperHeader(const std::string& encounterId, const std::string& patientId);

    std::list<VitalSignDetailDO> selectByVitalSignIds(const std::list<std::string>& ids);

    uint64_t countByBusNoPrefix(const std::string& prefix);

    std::list<VitalSignDO> selectBySlot(const std::string& encounterId, const std::string& patientId,
                                         const std::string& recordDate, const std::string& recordTime);

    int softDeleteById(const std::string& id);

    int deleteByVitalSignId(const std::string& vitalSignId);
};

#endif // !_NURSING_DAO_
