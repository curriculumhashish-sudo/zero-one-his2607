#pragma once

#ifndef _RENTCONTROLLER_DAO_
#define _RENTCONTROLLER_DAO_
#include "BaseDAO.h"
#include "../../domain/query/equipment/EquipmentQuery.h"
#include "../../domain/do/equipment/AdmDeviceDefinitionDO.h"
#include "../../domain/do/equipment/AdmChargeItemDefinitionDO.h" 
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
	std::list<EquipmentVeiwDO> selectWithPage(const EquipmentQuery::Wrapper& query);
	// 通过姓名查询数据
	std::list<AdmDeviceDefinitionDO> selectByName(const string& name);
	// 通过ID查询数据
	PtrEquipmentVeiwDO selectById(std::string id);

	// 新增的数据插入 (供 saveData 调用)
	int insert(const AdmDeviceDefinitionDO& entity);

	// 修改数据 (供 modifyData 调用)
	int update(const AdmDeviceDefinitionDO& entity);

	// 批量逻辑删除
	int softDeleteByIds(const std::list<std::string>& ids, const std::string& deletedBy, const std::string& deletedAt);

	// 批量修改状态
	int updateStatusByIds(const std::list<std::string>& ids, const std::string& statusEnum, const std::string& updatedBy, const std::string& updatedAt);

	//批量启停用
	int updateActiveFlagByIds(const std::list<std::string>& ids, int activeFlag, const std::string& statusEnum, const std::string& updatedBy, const std::string& updatedAt);

	//检查名称是否重复
	bool checkNameExists(const std::string& name, const std::string& excludeId);

	// 删除指定实例的所有价格记录 (供修改数据时，先清理旧价格使用)
	int deleteChargeItemsByInstanceId(const std::string& instanceId);

	// 插入单条收费项目定义 (价格信息)
	int insertChargeItem(const AdmChargeItemDefinitionDO& entity);

	// 插入或更新价格 (UPSERT，完美保留审计记录)
	int upsertChargeItem(const AdmChargeItemDefinitionDO& entity, const std::string& updatedAt, const std::string& updatedBy);

	// 删除被前端清空的价格
	int deleteRemovedPrices(const std::string& instanceId, const std::string& keepPurchase, const std::string& keepSale, const std::string& keepLimit);

	//获取最大bus_no编号
	uint64_t generateSequence(const std::string& seqKey);
};
#endif // !_RENTCONTROLLER_DAO_
