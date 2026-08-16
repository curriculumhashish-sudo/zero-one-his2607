#pragma once
#ifndef _DEPT_BATCH_STATUS_MAPPER_H_
#define _DEPT_BATCH_STATUS_MAPPER_H_

#include "Mapper.h"

/**
 * 映射父科室ID
 * 注意：executeQueryOne 已调用 next()，此处禁止再次 next()
 */
class ParentIdMapper : public Mapper<int64_t>
{
public:
	int64_t mapper(ResultSet* resultSet) const override
	{
		if (!resultSet || resultSet->isNull(1))
		{
			return 0;
		}
		return resultSet->getInt64(1);
	}
};

/**
 * 映射科室是否启用（查询结果为 0/1）
 * 仅依据 status_enum，不再使用已废弃的 active_flag
 */
class StatusActiveMapper : public Mapper<bool>
{
public:
	bool mapper(ResultSet* resultSet) const override
	{
		if (!resultSet)
		{
			return false;
		}
		return resultSet->getInt(1) == 1;
	}
};

/**
 * 映射单个子科室ID（配合 executeQuery 多行查询）
 */
class ChildIdMapper : public Mapper<int64_t>
{
public:
	int64_t mapper(ResultSet* resultSet) const override
	{
		return resultSet->getInt64(1);
	}
};

#endif // !_DEPT_BATCH_STATUS_MAPPER_H_