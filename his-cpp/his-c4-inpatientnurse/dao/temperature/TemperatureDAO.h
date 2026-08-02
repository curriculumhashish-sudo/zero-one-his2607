#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28 00:00:00

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
#ifndef _TEMPERATURE_DAO_
#define _TEMPERATURE_DAO_
#include "BaseDAO.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"


class TemperatureDAO : public BaseDAO
{
private:

    std::string queryConditionBuilder(const std::string& encounterId, const std::string& patientId,
                                      const std::string& dateFrom, const std::string& dateTo,
                                      SqlParams& params);

    std::string slotConditionBuilder(const std::string& encounterId, const std::string& patientId,
                                     const std::string& recordDate, const std::string& timeSlot,
                                     SqlParams& params);
public:

    uint64_t count(const std::string& encounterId, const std::string& patientId,
                   const std::string& dateFrom, const std::string& dateTo);

    std::list<VitalSignDO> selectByCondition(const std::string& encounterId, const std::string& patientId,
                                              const std::string& dateFrom, const std::string& dateTo);

    bool existsBySlot(const std::string& encounterId, const std::string& patientId,
                      const std::string& recordDate, const std::string& timeSlot);

    std::list<VitalSignDO> selectBySlot(const std::string& encounterId, const std::string& patientId,
                                         const std::string& recordDate, const std::string& timeSlot);

    uint64_t countByBusNoPrefix(const std::string& prefix);

    std::list<VitalSignDetailDO> selectByVitalSignIds(const std::list<std::string>& ids);

    int deleteByVitalSignId(const std::string& vitalSignId);
};

#endif // !_TEMPERATURE_DAO_
