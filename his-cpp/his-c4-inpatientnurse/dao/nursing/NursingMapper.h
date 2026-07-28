#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/25 14:00:00

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
#ifndef _NURSING_MAPPER_
#define _NURSING_MAPPER_

#include "Mapper.h"
#include "domain/do/nursing/VitalSignDO.h"
#include "domain/do/nursing/VitalSignDetailDO.h"

// Value-type mapper for list queries
class NursingMapper : public Mapper<VitalSignDO>
{
public:
    VitalSignDO mapper(ResultSet* resultSet) const override
    {
        VitalSignDO data;
        // Base columns
        data.setId(resultSet->getString("id"));
        data.setTenantId(resultSet->getString("tenant_id"));
        data.setCreatedAt(resultSet->getString("created_at"));
        data.setUpdatedAt(resultSet->getString("updated_at"));
        data.setCreatedBy(resultSet->getString("created_by"));
        data.setUpdatedBy(resultSet->getString("updated_by"));
        data.setIsDeleted(resultSet->getInt("is_deleted"));
        data.setDeletedAt(resultSet->getString("deleted_at"));
        data.setDeletedBy(resultSet->getString("deleted_by"));
        data.setVersion(resultSet->getInt("version"));
        // Business columns
        data.setPatientId(resultSet->getString("patient_id"));
        data.setEncounterId(resultSet->getString("encounter_id"));
        data.setRecordedAt(resultSet->getString("recorded_at"));
        data.setTypeEnum(resultSet->getString("type_enum"));
        data.setBusNo(resultSet->getString("bus_no"));
        data.setLocationId(resultSet->getString("location_id"));
        data.setBabyNo(resultSet->getInt("baby_no"));
        data.setRecorderId(resultSet->getString("recorder_id"));
        data.setRemark(resultSet->getString("remark"));
        return data;
    }
};

// Pointer-type mapper for single-record queries
class PtrNursingMapper : public Mapper<PtrVitalSignDO>
{
public:
    PtrVitalSignDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<VitalSignDO>();
        data->setId(resultSet->getString("id"));
        data->setTenantId(resultSet->getString("tenant_id"));
        data->setCreatedAt(resultSet->getString("created_at"));
        data->setUpdatedAt(resultSet->getString("updated_at"));
        data->setCreatedBy(resultSet->getString("created_by"));
        data->setUpdatedBy(resultSet->getString("updated_by"));
        data->setIsDeleted(resultSet->getInt("is_deleted"));
        data->setDeletedAt(resultSet->getString("deleted_at"));
        data->setDeletedBy(resultSet->getString("deleted_by"));
        data->setVersion(resultSet->getInt("version"));
        data->setPatientId(resultSet->getString("patient_id"));
        data->setEncounterId(resultSet->getString("encounter_id"));
        data->setRecordedAt(resultSet->getString("recorded_at"));
        data->setTypeEnum(resultSet->getString("type_enum"));
        data->setBusNo(resultSet->getString("bus_no"));
        data->setLocationId(resultSet->getString("location_id"));
        data->setBabyNo(resultSet->getInt("baby_no"));
        data->setRecorderId(resultSet->getString("recorder_id"));
        data->setRemark(resultSet->getString("remark"));
        return data;
    }
};

// Value-type mapper for detail list queries
class NursingDetailMapper : public Mapper<VitalSignDetailDO>
{
public:
    VitalSignDetailDO mapper(ResultSet* resultSet) const override
    {
        VitalSignDetailDO data;
        data.setId(resultSet->getString("id"));
        data.setTenantId(resultSet->getString("tenant_id"));
        data.setCreatedAt(resultSet->getString("created_at"));
        data.setUpdatedAt(resultSet->getString("updated_at"));
        data.setCreatedBy(resultSet->getString("created_by"));
        data.setUpdatedBy(resultSet->getString("updated_by"));
        data.setIsDeleted(resultSet->getInt("is_deleted"));
        data.setDeletedAt(resultSet->getString("deleted_at"));
        data.setDeletedBy(resultSet->getString("deleted_by"));
        data.setVersion(resultSet->getInt("version"));
        data.setVitalSignId(resultSet->getString("vital_sign_id"));
        data.setTermCode(resultSet->getString("term_code"));
        data.setItemCode(resultSet->getString("item_code"));
        data.setItemValue(resultSet->getString("item_value"));
        data.setUnits(resultSet->getString("units"));
        data.setRemark(resultSet->getString("remark"));
        return data;
    }
};

#endif // !_NURSING_MAPPER_
