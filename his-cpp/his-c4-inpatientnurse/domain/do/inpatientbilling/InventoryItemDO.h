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
#ifndef _INVENTORYITEMDO_H_
#define _INVENTORYITEMDO_H_

#include "../DoInclude.h"

// Complete mapping for database table wkf_inventory_item.
class InventoryItemDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
	MYSQL_SYNTHESIZE(string, itemId, ItemId);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, inventoryStatusEnum, InventoryStatusEnum);
	MYSQL_SYNTHESIZE(string, supplierId, SupplierId);
	MYSQL_SYNTHESIZE(string, descriptionText, DescriptionText);
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	MYSQL_SYNTHESIZE(string, characteristicJson, CharacteristicJson);
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	MYSQL_SYNTHESIZE(string, productionDate, ProductionDate);
	MYSQL_SYNTHESIZE(string, expirationDate, ExpirationDate);
	MYSQL_SYNTHESIZE(int, validityMon, ValidityMon);
	MYSQL_SYNTHESIZE(string, locationStoreId, LocationStoreId);
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
	MYSQL_SYNTHESIZE(int, packagingLevels, PackagingLevels);

public:
	InventoryItemDO() : BaseDO("wkf_inventory_item")
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
		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, true);
		MYSQL_ADD_FIELD("item_table", "s", itemTable);
		MYSQL_ADD_FIELD("item_id", "s", itemId);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD("inventory_status_enum", "s", inventoryStatusEnum);
		MYSQL_ADD_FIELD("supplier_id", "s", supplierId);
		MYSQL_ADD_FIELD_NULLABLE("description_text", "s", descriptionText, true);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD_NULLABLE("characteristic_json", "s", characteristicJson, true);
		MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
		MYSQL_ADD_FIELD_NULLABLE("production_date", "dt", productionDate, true);
		MYSQL_ADD_FIELD_NULLABLE("expiration_date", "dt", expirationDate, true);
		MYSQL_ADD_FIELD_NULLABLE("validity_mon", "i", validityMon, true);
		MYSQL_ADD_FIELD_NULLABLE("location_store_id", "s", locationStoreId, true);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD_NULLABLE("trace_no", "s", traceNo, true);
		MYSQL_ADD_FIELD_NULLABLE("packaging_levels", "i", packagingLevels, true);
	}
};

typedef std::shared_ptr<InventoryItemDO> PtrInventoryItemDO;

#endif // _INVENTORYITEMDO_H_
