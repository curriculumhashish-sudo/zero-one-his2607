#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/27 00:00:00

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
#ifndef _CHARGEITEMDEFINITIONDO_H_
#define _CHARGEITEMDEFINITIONDO_H_

#include "../DoInclude.h"

// Complete mapping for database table adm_charge_item_definition.
class ChargeItemDefinitionDO : public BaseDO
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
	ChargeItemDefinitionDO() : BaseDO("adm_charge_item_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD_NULLABLE("instance_table", "s", instanceTable, true);
		MYSQL_ADD_FIELD("instance_id", "s", instanceId);
		MYSQL_ADD_FIELD_NULLABLE("charge_name", "s", chargeName, true);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD_NULLABLE("yb_type", "s", ybType, true);
		MYSQL_ADD_FIELD("price_type_code", "s", priceTypeCode);
		MYSQL_ADD_FIELD_NULLABLE("lot_no", "s", lotNo, true);
		MYSQL_ADD_FIELD("price_value", "d", priceValue);
		MYSQL_ADD_FIELD_NULLABLE("priority", "i", priority, true);
	}
};

typedef std::shared_ptr<ChargeItemDefinitionDO> PtrChargeItemDefinitionDO;

#endif // _CHARGEITEMDEFINITIONDO_H_
