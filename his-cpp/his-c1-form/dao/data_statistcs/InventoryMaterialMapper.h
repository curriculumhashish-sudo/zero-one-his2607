#pragma once
#ifndef _INVENTORYMATERIALMAPPER_H_
#define _INVENTORYMATERIALMAPPER_H_

#include "../../domain/do/data_statistics/InventoryMaterialDO.h"

// ◊÷∂Œ∆•≈‰”≥…‰
class InventoryMaterialMapper : public Mapper<InventoryMaterialDO> {
public:
	InventoryMaterialDO mapper(ResultSet* resultSet) const override {
		InventoryMaterialDO inventoryMaterialDO;
		inventoryMaterialDO.setId(resultSet->getString("id"));
		inventoryMaterialDO.setCreatedBy(resultSet->getString("created_by"));
		inventoryMaterialDO.setUpdatedBy(resultSet->getString("updated_by"));
		inventoryMaterialDO.setBusNo(resultSet->getString("bus_no"));
		return inventoryMaterialDO;
	}
};

#endif // _INVENTORYMATERIALMAPPER_H_
