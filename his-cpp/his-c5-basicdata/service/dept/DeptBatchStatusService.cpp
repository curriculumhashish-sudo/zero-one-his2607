#include "DeptBatchStatusService.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"
#include <unordered_set>

BatchResult DeptBatchStatusService::batchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& reqDto)
{
	BatchResult result;

	// 1. 字符串ID转数字ID并去重
	std::vector<int64_t> numIdList;
	std::unordered_set<int64_t> uniqueIdSet;

	if (reqDto->deptIdList != nullptr && !reqDto->deptIdList->empty())
	{
		for (const auto& strId : *(reqDto->deptIdList))
		{
			try
			{
				std::string idStr(strId->c_str());
				int64_t idNum = std::stoll(idStr);
				if (uniqueIdSet.find(idNum) == uniqueIdSet.end())
				{
					uniqueIdSet.insert(idNum);
					numIdList.push_back(idNum);
				}
			}
			catch (...)
			{
				result.success = false;
				result.errorMsg = ZH_WORDS_GETTER("dept.batch-status.error.id-convert-fail");
				return result;
			}
		}
	}

	// 2. 父子科室状态规则校验
	DeptBatchStatusDAO deptDao;
	std::string validateError;
	if (!validateParentChildStatus(numIdList, reqDto->targetStatus, validateError))
	{
		result.success = false;
		result.errorMsg = validateError;
		return result;
	}

	// 3. 组装更新DTO（仅维护 status_enum）
	auto updateDto = DeptBatchUpdateDTO::createShared();
	updateDto->numIdList = numIdList;
	updateDto->statusEnum = (reqDto->targetStatus == 1) ? "ACTIVE" : "INACTIVE";
	updateDto->updateBy = reqDto->getPayload()->getUsername();
	updateDto->updateTime = SimpleDateTimeFormat::format();

	// 4. 执行批量更新（失败整批回滚）
	int affectRows = 0;

	if (reqDto->targetStatus == 1)
	{
		// 启用：先更新本次列表，再级联启用子科室
		affectRows = deptDao.batchUpdateStatus(numIdList, updateDto);
		if (affectRows == static_cast<int>(numIdList.size()))
		{
			bool cascadeSuccess = cascadeEnableChildren(numIdList, updateDto->updateBy, updateDto->updateTime);
			if (!cascadeSuccess)
			{
				result.success = false;
				result.errorMsg = ZH_WORDS_GETTER("dept.batch-status.error.cascade-fail");
				return result;
			}
		}
	}
	else
	{
		// 停用：仅更新本次列表（启用中的子科室需已停用或同批停用）
		affectRows = deptDao.batchUpdateStatus(numIdList, updateDto);
	}

	// 5. 组装结果
	if (affectRows == static_cast<int>(numIdList.size()))
	{
		result.success = true;
		result.successCount = affectRows;
		result.failCount = 0;
	}
	else if (affectRows == 0)
	{
		result.success = false;
		result.errorMsg = ZH_WORDS_GETTER("dept.batch-status.error.no-match");
		result.failCount = static_cast<int>(numIdList.size());
	}
	else
	{
		result.success = false;
		result.errorMsg = ZH_WORDS_GETTER("dept.batch-status.error.partial-fail");
		result.successCount = affectRows;
		result.failCount = static_cast<int>(numIdList.size()) - affectRows;
	}

	return result;
}

bool DeptBatchStatusService::validateParentChildStatus(const std::vector<int64_t>& deptIdList, int targetStatus, std::string& errorMsg)
{
	DeptBatchStatusDAO deptDao;
	std::unordered_set<int64_t> idSet(deptIdList.begin(), deptIdList.end());

	for (int64_t deptId : deptIdList)
	{
		if (targetStatus == 0)
		{
			// 规则1：存在启用子科室则禁止停用父科室；同批一并停用的子科室除外
			if (deptDao.hasActiveChildDept(deptId))
			{
				auto childIds = deptDao.getChildDeptIds(deptId);
				for (int64_t childId : childIds)
				{
					if (!deptDao.isDeptActive(childId))
					{
						continue;
					}
					if (idSet.find(childId) == idSet.end())
					{
						errorMsg = ZH_WORDS_GETTER("dept.batch-status.error.has-active-child");
						return false;
					}
				}
			}
		}
		else
		{
			// 规则3：父科室未启用则禁止启用子科室；同批一并启用的父科室除外
			int64_t parentId = deptDao.getParentDeptId(deptId);
			if (parentId != 0)
			{
				bool parentInList = idSet.find(parentId) != idSet.end();
				if (!parentInList && !deptDao.isDeptActive(parentId))
				{
					errorMsg = ZH_WORDS_GETTER("dept.batch-status.error.parent-inactive");
					return false;
				}
			}
		}
	}
	return true;
}

bool DeptBatchStatusService::cascadeEnableChildren(const std::vector<int64_t>& parentIds, const std::string& updateBy, const std::string& updateTime)
{
	DeptBatchStatusDAO deptDao;

	// 规则2：启用父科室时，级联启用所有直接子科室
	std::vector<int64_t> needEnableIds;
	std::unordered_set<int64_t> childIdSet;
	for (int64_t parentId : parentIds)
	{
		auto childIds = deptDao.getChildDeptIds(parentId);
		for (int64_t childId : childIds)
		{
			if (!childIdSet.insert(childId).second)
			{
				continue;
			}
			// 已启用的跳过（含本次批量已更新的）
			if (!deptDao.isDeptActive(childId))
			{
				needEnableIds.push_back(childId);
			}
		}
	}

	if (needEnableIds.empty())
	{
		return true;
	}

	auto updateDto = DeptBatchUpdateDTO::createShared();
	updateDto->numIdList = needEnableIds;
	updateDto->statusEnum = "ACTIVE";
	updateDto->updateBy = updateBy;
	updateDto->updateTime = updateTime;

	int affectRows = deptDao.batchUpdateStatus(needEnableIds, updateDto);
	return affectRows == static_cast<int>(needEnableIds.size());
}