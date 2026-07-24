#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

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
#ifndef _DRUG_PRICE_DO_H_
#define _DRUG_PRICE_DO_H_

#include "../DoInclude.h"

/**
 * Persistence model for drug prices in adm_charge_item_definition.
 */
class DrugPriceDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);
    MYSQL_SYNTHESIZE(string, instanceTable, InstanceTable);
    MYSQL_SYNTHESIZE(string, instanceId, InstanceId);
    MYSQL_SYNTHESIZE(string, chargeName, ChargeName);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, orgId, OrgId);
    MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
    MYSQL_SYNTHESIZE(string, ybType, YbType);
    MYSQL_SYNTHESIZE(string, priceTypeCode, PriceTypeCode);
    MYSQL_SYNTHESIZE(string, lotNo, LotNo);
    MYSQL_SYNTHESIZE(double, priceValue, PriceValue);
    MYSQL_SYNTHESIZE(int, priority, Priority);

  public:
    DrugPriceDO() : BaseDO("adm_charge_item_definition")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
        MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
        MYSQL_ADD_FIELD("created_by", "s", createdBy);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
        MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
        MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
        MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
        MYSQL_ADD_FIELD("instance_table", "s", instanceTable);
        MYSQL_ADD_FIELD_NULLABLE("instance_id", "s", instanceId, false);
        MYSQL_ADD_FIELD("charge_name", "s", chargeName);
        MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
        MYSQL_ADD_FIELD_NULLABLE("type_code", "s", typeCode, false);
        MYSQL_ADD_FIELD("yb_type", "s", ybType);
        MYSQL_ADD_FIELD_NULLABLE("price_type_code", "s", priceTypeCode, false);
        MYSQL_ADD_FIELD("lot_no", "s", lotNo);
        MYSQL_ADD_FIELD_NULLABLE("price_value", "d", priceValue, false);
        MYSQL_ADD_FIELD("priority", "i", priority);
    }
};

using PtrDrugPriceDO = std::shared_ptr<DrugPriceDO>;

#endif // !_DRUG_PRICE_DO_H_
