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
#ifndef _PHARM_DO_
#define _PHARM_DO_
#include "../DoInclude.h"

/**
 * PharmDO - Pharmacy/warehouse domain object mapped to adm_location table.
 *
 * Pharmacies and warehouses are stored in adm_location, distinguished by
 * form_enum (e.g. 'PHARMACY' for pharmacy, 'WAREHOUSE' for warehouse).
 */
class PharmDO : public BaseDO
{
    // Primary key (VARCHAR(64) string)
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // Tenant identifier
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
    // Pharmacy/warehouse name
    MYSQL_SYNTHESIZE(std::string, name, Name);
    // Business number
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
    // Publication status (ACTIVE / INACTIVE)
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    // Operational status
    MYSQL_SYNTHESIZE(std::string, operationalEnum, OperationalEnum);
    // Location form: PHARMACY / WAREHOUSE
    MYSQL_SYNTHESIZE(std::string, formEnum, FormEnum);
    // Location mode
    MYSQL_SYNTHESIZE(std::string, modeEnum, ModeEnum);
    // Type JSON snapshot
    MYSQL_SYNTHESIZE(std::string, typeJson, TypeJson);
    // Organization identifier
    MYSQL_SYNTHESIZE(std::string, organizationId, OrganizationId);
    // Display sort order
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
    // Pinyin search code
    MYSQL_SYNTHESIZE(std::string, pyStr, PyStr);
    // Wubi search code
    MYSQL_SYNTHESIZE(std::string, wbStr, WbStr);
    // Parent location identifier (nullable)
    MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
    // Audit fields
    MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);

public:
    PharmDO() : BaseDO("adm_location")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD_NULLABLE("operational_enum", "s", operationalEnum, true);
        MYSQL_ADD_FIELD("form_enum", "s", formEnum);
        MYSQL_ADD_FIELD("mode_enum", "s", modeEnum);
        MYSQL_ADD_FIELD_NULLABLE("type_json", "s", typeJson, true);
        MYSQL_ADD_FIELD_NULLABLE("organization_id", "s", organizationId, true);
        MYSQL_ADD_FIELD_NULLABLE("display_order", "i", displayOrder, true);
        MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
        MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
        MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId, true);
        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
        MYSQL_ADD_FIELD("created_at", "dt", createdAt);
        MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
    }
};

typedef std::shared_ptr<PharmDO> PtrPharmDO;

#endif // !_PHARM_DO_
