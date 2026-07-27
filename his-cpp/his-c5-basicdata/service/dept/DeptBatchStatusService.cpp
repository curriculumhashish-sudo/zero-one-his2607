#include "DeptBatchStatusService.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"
#include <algorithm>
#include <unordered_set>

BatchResult DeptBatchStatusService::batchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& reqDto)
{
    BatchResult result;

    // 1. 转换字符串ID为数字ID（去重）
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

    // 2. 校验父子科室状态规则
    DeptBatchStatusDAO deptDao;
    std::string validateError;
    if (!validateParentChildStatus(numIdList, reqDto->targetStatus, validateError))
    {
        result.success = false;
        result.errorMsg = validateError;
        return result;
    }

    // 3. 组装更新DTO
    auto updateDto = DeptBatchUpdateDTO::createShared();
    updateDto->numIdList = numIdList;

    if (reqDto->targetStatus == 1)
    {
        updateDto->statusEnum = "ACTIVE";
    }
    else
    {
        updateDto->statusEnum = "INACTIVE";
    }

    updateDto->updateBy = reqDto->getPayload()->getUsername();
    updateDto->updateTime = SimpleDateTimeFormat::format();

    // 4. 执行批量更新
    int affectRows = 0;

    if (reqDto->targetStatus == 1)
    {
        // ========== 启用操作 ==========
        // 先更新父科室
        affectRows = deptDao.batchUpdateStatus(numIdList, updateDto);

        // 再级联启用所有子科室（不管之前状态如何，全部启用）
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
        // ========== 停用操作 ==========
        affectRows = deptDao.batchUpdateStatus(numIdList, updateDto);
    }

    // 5. 判断结果
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

    // 先检查是否存在父子关系冲突
    for (int64_t deptId : deptIdList)
    {
        if (targetStatus == 0)
        {
            // 规则1：存在启用子节点，不可停用父节点
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
            // 规则2：父节点未启用，子节点不可启用
            int64_t parentId = deptDao.getParentDeptId(deptId);
            if (parentId != 0)  // 有父科室
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

    // Rule 2: enabling parent cascades to all direct children
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
            // Skip children already active (e.g. already updated in this batch)
            if (!deptDao.isDeptActive(childId))
            {
                needEnableIds.push_back(childId);
            }
        }
    }

    if (needEnableIds.empty())
    {
        return true;  // 没有子科室，无需级联
    }

    // 级联启用所有子科室
    auto updateDto = DeptBatchUpdateDTO::createShared();
    updateDto->numIdList = needEnableIds;
    updateDto->statusEnum = "ACTIVE";
    updateDto->updateBy = updateBy;
    updateDto->updateTime = updateTime;

    int affectRows = deptDao.batchUpdateStatus(needEnableIds, updateDto);
    return affectRows == static_cast<int>(needEnableIds.size());
}
