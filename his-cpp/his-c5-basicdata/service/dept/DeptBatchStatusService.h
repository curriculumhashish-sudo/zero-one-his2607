#pragma once
#ifndef _DEPT_BATCH_STATUS_SERVICE_H_
#define _DEPT_BATCH_STATUS_SERVICE_H_

#include "../../domain/dto/dept/DeptBatchStatusDTO.h"
#include "../../dao/dept/DeptBatchStatusDAO.h"
#include <string>
#include <vector>

/**
 * 批量启停操作结果
 */
struct BatchResult
{
	bool success;
	std::string errorMsg;
	int successCount;
	int failCount;

	BatchResult() : success(false), successCount(0), failCount(0) {}
};

/**
 * 科室批量启停业务层
 */
class DeptBatchStatusService
{
public:
	/**
	 * 批量启停科室
	 * @param reqDto 前端请求DTO
	 * @return 批量操作结果（失败时整批回滚，不支持部分成功）
	 */
	BatchResult batchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& reqDto);

private:
	/**
	 * 校验父子科室状态规则
	 * 规则1：存在启用子科室时不可停用父科室（同批一并停用的子科室除外）
	 * 规则2：启用父科室后级联启用直接子科室（见 cascadeEnableChildren）
	 * 规则3：父科室未启用时不可启用子科室（同批一并启用的父科室除外）
	 */
	bool validateParentChildStatus(const std::vector<int64_t>& deptIdList, int targetStatus, std::string& errorMsg);

	/**
	 * 级联启用直接子科室
	 */
	bool cascadeEnableChildren(const std::vector<int64_t>& parentIds, const std::string& updateBy, const std::string& updateTime);
};

#endif