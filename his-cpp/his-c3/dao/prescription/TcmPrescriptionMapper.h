#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 Defines result-set mappers used by TCM prescription database operations.

 @Author: nick1484065161
 @Date: 2026/07/23 00:00:00

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

// Prevents duplicate mapper declarations when this header is included repeatedly.
#ifndef _TCM_PRESCRIPTION_MAPPER_H_
#define _TCM_PRESCRIPTION_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/prescription/MedMedicationRequestDO.h"

/**
 * Maps an ordered medication request query result to its data object.
 */
class PtrMedMedicationRequestMapper
    : public Mapper<PtrMedMedicationRequestDO>
{
public:
    PtrMedMedicationRequestDO mapper(
        ResultSet* resultSet
    ) const override
    {
        auto data = std::make_shared<MedMedicationRequestDO>();
        data->setId(resultSet->getString(1));
        data->setTenantId(resultSet->getString(2));
        data->setVersion(resultSet->getInt(3));
        data->setPrescriptionNo(resultSet->getString(4));
        data->setGroupId(resultSet->getString(5));
        data->setStatusEnum(resultSet->getString(6));
        data->setStatusReason(resultSet->getInt(7));
        data->setStatusChangedTime(resultSet->getString(8));
        data->setPatientId(resultSet->getString(9));
        data->setEncounterId(resultSet->getString(10));
        data->setEffectiveDoseEnd(resultSet->getString(11));
        data->setTherapyEnum(resultSet->getString(12));
        data->setTcmFlag(resultSet->getInt(13));
        data->setContentJson(resultSet->getString(14));
        data->setPrescriptionGroupKey(resultSet->getString(15));
        data->setIsDeleted(resultSet->getInt(16));
        data->setUpdatedBy(resultSet->getString(17));
        data->setUpdatedAt(resultSet->getString(18));
        return data;
    }
};

/**
 * Maps the first column of a single-row query result to an integer value.
 */
class TcmIntegerMapper : public Mapper<int>
{
public:
    int mapper(ResultSet* resultSet) const override
    {
        return resultSet->getInt(1);
    }
};

#endif // !_TCM_PRESCRIPTION_MAPPER_H_
