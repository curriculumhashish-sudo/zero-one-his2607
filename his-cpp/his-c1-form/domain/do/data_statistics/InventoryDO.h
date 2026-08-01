#pragma once
#ifndef _INVENTORYDO_H_
#define _INVENTORYDO_H_

#include "InventoryMaterialDO.h"
#include "InOutboundDo.h"

class InventoryDO {
public:
	InventoryMaterialDO inventoryMaterialDO;

	InOutboundDO inOutboundDO;
};

#endif // !_INVENTORYDO_H_
