#pragma once
#ifndef _INVENTORYMATERIALSERVICE_H_
#define _INVENTORYMATERIALSERVICE_H_

#include "domain/vo/data_statistics/InventoryMaterialVo.h"
#include "domain/query/data_statistics/InventoryMaterialQuery.h"
#include "domain/dto/data_statistics/InventoryMaterialDto.h"

class InventoryMaterialService {
public:
	// 分页查询所有数据
	InventoryMaterialPageDto::Wrapper listAll(const InventoryMaterialQuery::Wrapper& query);
	// 通过ID查询一条数据
	InventoryMaterialDto::Wrapper getById(std::string id);
};

#endif // !_INVENTORYMATERIALSERVICE_H_