#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xinpingqihe
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
#ifndef _CHARGEITEMDEFINITIONDO_H_
#define _CHARGEITEMDEFINITIONDO_H_

#include "../DoInclude.h"

class ChargeItemDefinitionDO : public BaseDO {
	// Primary key
	MYSQL_SYNTHESIZE(std::string, id, Id);
	// Tenant id
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	// Created at
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	// Updated at
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	// Created by
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	// Updated by
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	// Is deleted
	MYSQL_SYNTHESIZE(std::string, isDeleted, IsDeleted);
	// Deleted at
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	// Deleted by
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
	// Version
	MYSQL_SYNTHESIZE(std::string, version, Version);
	// Instance table: the business entity owning this definition
	MYSQL_SYNTHESIZE(std::string, instanceTable, InstanceTable);
	// Instance id: business instance primary key
	MYSQL_SYNTHESIZE(std::string, instanceId, InstanceId);
	// Charge display name
	MYSQL_SYNTHESIZE(std::string, chargeName, ChargeName);
	// Status enum (PublicationStatus, default ACTIVE)
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	// Organization id
	MYSQL_SYNTHESIZE(std::string, orgId, OrgId);
	// Type code (fee type code)
	MYSQL_SYNTHESIZE(std::string, typeCode, TypeCode);
	// Insurance type (yb_type)
	MYSQL_SYNTHESIZE(std::string, ybType, YbType);
	// Price type code
	MYSQL_SYNTHESIZE(std::string, priceTypeCode, PriceTypeCode);
	// Lot number / batch number
	MYSQL_SYNTHESIZE(std::string, lotNo, LotNo);
	// Price value (in yuan)
	MYSQL_SYNTHESIZE(double, priceValue, PriceValue);
	// Priority (lower value = higher priority)
	MYSQL_SYNTHESIZE(int, priority, Priority);

public:
	ChargeItemDefinitionDO() : BaseDO("adm_charge_item_definition") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "s", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "s", version);
		MYSQL_ADD_FIELD("instance_table", "s", instanceTable);
		MYSQL_ADD_FIELD("instance_id", "s", instanceId);
		MYSQL_ADD_FIELD("charge_name", "s", chargeName);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("org_id", "s", orgId);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD("yb_type", "s", ybType);
		MYSQL_ADD_FIELD("price_type_code", "s", priceTypeCode);
		MYSQL_ADD_FIELD("lot_no", "s", lotNo);
		MYSQL_ADD_FIELD("price_value", "d", priceValue);
		MYSQL_ADD_FIELD("priority", "i", priority);
	}
};

typedef std::shared_ptr<ChargeItemDefinitionDO> PtrChargeItemDefinitionDO;

#endif // !_CHARGEITEMDEFINITIONDO_H_
