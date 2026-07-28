#pragma once

#ifndef _EQUIPMENT_SERVICE_
#define _EQUIPMENT_SERVICE_
#include "domain/vo/equipment/EquipmentVO.h"
#include "domain/query/equipment/EquipmentQuery.h"
#include "domain/dto/equipment/EquipmentAddDTO.h"
#include "domain/dto/equipment/EquipmentStatusDTO.h"
#include "domain/dto/equipment/EquipmentDeleteDTO.h"
#include "domain/dto/equipment/EquipmentDetailDTO.h"
#include "domain/dto/equipment/EquipmentModifyDTO.h"
#include "domain/dto/equipment/EquipmentSelectDTO.h"
/**
 * 示例服务实现，演示基础的示例服务实现
 */
class EquipmentService
{
public:
	// 分页查询所有数据
	EquipmentDetailPageDTO::Wrapper listAll(const EquipmentQuery::Wrapper& query);
	// 通过ID查询单个数据
	EquipmentDTO::Wrapper getById(std::string id);
	// 保存数据
	std::string saveData(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 批量保存数据
	// bool saveDataBatch(const oatpp::List<SampleAddDTO::Wrapper>& datas);
	// 修改数据
	// bool updateData(const SampleDTO::Wrapper& dto);
	// 通过ID删除数据
	// bool removeData(const oatpp::List<oatpp::String>& ids);

	// 修改数据
	bool modifyData(const EquipmentModifyDTO::Wrapper& dto);
	// 批量删除
	bool deleteData(const EquipmentDeleteDTO::Wrapper& dto);
	// 批量启停用
	bool changeStatus(const EquipmentStatusDTO::Wrapper& dto);
	// 列表选择查询
	EquipmentSelectPageDTO::Wrapper selectData(const EquipmentQuery::Wrapper& query);
};

#endif // !_EQUIPMENT_SERVICE_

