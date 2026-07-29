#pragma once

#ifndef _RENTCONTROLLER_DAO_
#define _RENTCONTROLLER_DAO_
#include "BaseDAO.h"
#include "../../domain/do/equipment/DocInventoryItemSnapshotDO.h"
#include "../../domain/do/equipment/InventoryLockDO.h"
#include "../../domain/do/equipment/WkfInventoryItemDO.h"
#include "../../domain/query/equipment/EquipmentQuery.h"

/**
 * 示例表数据库操作实现
 */
class EquipmentDAO : public BaseDAO
{
private:
	//************************************
	// Method:      queryConditionBuilder
	// FullName:    SampleDAO::queryConditionBuilder
	// Access:      private 
	// Returns:     std::string 返回条件SQL语句，即WHERE部分的语句
	// Parameter:   const SampleQuery::Wrapper& query 查询数据对象
	// Parameter:   SqlParams& params 存放查询数据的参数对象
	// Description: 查询条件构建器，根据SampleQuery对象动态构建查询条件相关参数
	//************************************
	inline std::string queryConditionBuilder(const EquipmentQuery::Wrapper& query, SqlParams& params);
public:
	// 统计数据条数
	uint64_t count(const EquipmentQuery::Wrapper& query);
	// 分页查询数据
	std::list<WkfInventoryItemViewDO> selectWithPage(const EquipmentQuery::Wrapper& query);
	// 通过姓名查询数据
	std::list<WkfInventoryItemDO> selectByName(const string& name);
	// 通过ID查询数据
	PtrWkfInventoryItemDO selectById(std::string id);

	// 新增的数据插入 (供 saveData 调用)
	int insert(const WkfInventoryItemDO& entity);

	// 修改数据 (供 modifyData 调用)
	int update(const WkfInventoryItemDO& entity);

	// 批量逻辑删除
	int softDeleteByIds(const std::list<std::string>& ids, const std::string& deletedBy, const std::string& deletedAt);

	// 批量修改状态
	int updateStatusByIds(const std::list<std::string>& ids, const std::string& statusEnum, const std::string& updatedBy, const std::string& updatedAt);
};
#endif // !_RENTCONTROLLER_DAO_
