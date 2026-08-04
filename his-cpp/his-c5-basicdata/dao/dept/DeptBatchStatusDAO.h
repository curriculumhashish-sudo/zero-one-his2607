#pragma once
#ifndef _DEPT_BATCH_STATUS_DAO_H_
#define _DEPT_BATCH_STATUS_DAO_H_

#include "BaseDAO.h"
#include "../../domain/dto/dept/DeptBatchStatusDTO.h"
#include <vector>

/**
 * 科室批量启停数据访问层
 * 状态字段统一使用 status_enum（ACTIVE / INACTIVE），不涉及 active_flag
 */
class DeptBatchStatusDAO : public BaseDAO
{
public:
	~DeptBatchStatusDAO() override = default;

	/**
	 * 批量更新科室状态（事务：全成功提交，否则回滚）
	 * @param numIdList 数字ID列表
	 * @param updateDto 更新数据（statusEnum / updateBy / updateTime）
	 * @return 影响行数；失败返回 0
	 */
	int batchUpdateStatus(const std::vector<int64_t>& numIdList, const DeptBatchUpdateDTO::Wrapper& updateDto);

	/**
	 * 查询父科室ID
	 * @return 父科室ID；无父科室或 parent_id 为空时返回 0
	 */
	int64_t getParentDeptId(int64_t deptId);

	/**
	 * 是否存在启用状态的子科室（status_enum = ACTIVE）
	 */
	bool hasActiveChildDept(int64_t deptId);

	/**
	 * 查询直接子科室ID列表
	 */
	std::vector<int64_t> getChildDeptIds(int64_t deptId);

	/**
	 * 科室是否为启用状态（status_enum = ACTIVE）
	 */
	bool isDeptActive(int64_t deptId);
};

#endif