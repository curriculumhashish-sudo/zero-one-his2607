// ======================================================
// GetCategoryDetailsMapper.h - 费用大类明细 - 结果集映射器
// Mapper 的工作流程：
// 1. DAO 执行 SQL → 产生 ResultSet（N行）
// 2. DAO 调用 sqlSession->executeQuery<GetCategoryDetailDO>(sql, GetCategoryDetailsMapper(), params)
// 3. 框架对 ResultSet 每行调用 mapper(resultSet) → 返回一个 GetCategoryDetailDO
// 4. 所有 DO 收集到 std::list<GetCategoryDetailDO> 返回给 DAO
// ======================================================
#pragma once
#include "Mapper.h"
#include "../../domain/do/settle/GetCategoryDetailsDO.h"

// 值语义Mapper - 用于列表查询，返回 std::list<GetCategoryDetailDO>
class GetCategoryDetailsMapper : public Mapper<GetCategoryDetailDO>
{
public:
	GetCategoryDetailDO mapper(ResultSet* resultSet) const override
	{
		GetCategoryDetailDO data;
		data.setCharge_type_name(resultSet->getString("charge_type_name"));
		data.setType_total_fee(resultSet->getDouble("type_total_fee"));
		return data;
	}
};

// 指针语义Mapper – 用于需要 shared_ptr 的场景
class PtrGetCategoryDetailsMapper : public Mapper<PtrGetCategoryDetailDO>
{
public:
	PtrGetCategoryDetailDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<GetCategoryDetailDO>();
		data->setCharge_type_name(resultSet->getString("charge_type_name"));
		data->setType_total_fee(resultSet->getDouble("type_total_fee"));
		return data;
	}
};
