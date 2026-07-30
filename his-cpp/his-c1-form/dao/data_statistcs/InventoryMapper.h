#pragma once
#ifndef _INVENTORYMAPPER_H_
#define _INVENTORYMAPPER_H_

#include "../../domain/do/data_statistics/InventoryDO.h"

class InventoryMapper : public Mapper<InventoryDO> {
public:
	InventoryDO mapper(ResultSet* resultSet) const override {
		InventoryDO inventoryDO;
		inventoryDO.inventoryMaterialDO.setId(resultSet->getString("id"));
		inventoryDO.inventoryMaterialDO.setCreatedBy(resultSet->getString("created_by"));
		inventoryDO.inventoryMaterialDO.setUpdatedBy(resultSet->getString("updated_by"));
		inventoryDO.inventoryMaterialDO.setBusNo(resultSet->getString("bus_no"));
		inventoryDO.inOutboundDO.setQuantity(resultSet->getDouble("quantity"));
		inventoryDO.inOutboundDO.setName(resultSet->getString("name"));
		inventoryDO.inOutboundDO.setProductionDate(resultSet->getString("production_date"));
		inventoryDO.inOutboundDO.setExpirationDate(resultSet->getString("expiration_date"));
		inventoryDO.inOutboundDO.setRemainingDays(resultSet->getInt("remaining_days"));
		return inventoryDO;
	}
};

#endif
