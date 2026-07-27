#include "DeptBatchStatusService.h"
#include "SimpleDateTimeFormat.h"
#include "stdafx.h"
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
                result.errorMsg = "科室ID格式转换失败: " + std::string(strId->c_str());
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
        // 启用操作：先更新父科室
        affectRows = deptDao.batchUpdateStatus(numIdList, updateDto);

        // 再级联启用所有子科室
        if (affectRows == static_cast<int>(numIdList.size()))
        {
            bool cascadeSuccess = cascadeEnableChildren(numIdList, updateDto->updateBy, updateDto->updateTime);
            if (!cascadeSuccess)
            {
                result.success = false;
                result.errorMsg = "级联启用子科室失败，已全部回滚";
                return result;
            }
        }
    }
    else
    {
        // 停用操作
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
        result.errorMsg = "批量操作失败，无匹配科室数据，请检查科室ID是否正确或科室是否已被删除";
        result.failCount = static_cast<int>(numIdList.size());
    }
    else
    {
        result.success = false;
        result.errorMsg = "批量操作部分失败，已全部回滚";
        result.successCount = affectRows;
        result.failCount = static_cast<int>(numIdList.size()) - affectRows;
    }

    return result;
}

bool DeptBatchStatusService::validateParentChildStatus(const std::vector<int64_t>& deptIdList, int targetStatus, std::string& errorMsg)
{
    DeptBatchStatusDAO deptDao;

    for (int64_t deptId : deptIdList)
    {
        if (targetStatus == 0)  // 停用操作
        {
            // 规则1：存在启用子节点，不可停用父节点
            if (deptDao.hasActiveChildDept(deptId))
            {
                errorMsg = "科室ID [" + std::to_string(deptId) + "] 存在启用的子科室，请先停用子科室";
                return false;
            }
        }
        else  // 启用操作
        {
            // 规则2：父节点未启用，子节点不可启用（只对非根节点校验）
            int64_t parentId = deptDao.getParentDeptId(deptId);
            if (parentId != 0)  // 有父科室
            {
                // 检查父科室是否也在本次批量列表中（如果在，后续会一起启用）
                bool parentInList = std::find(deptIdList.begin(), deptIdList.end(), parentId) != deptIdList.end();
                if (!parentInList && !deptDao.isDeptActive(parentId))
                {
                    errorMsg = "科室ID [" + std::to_string(deptId) + "] 的父科室未启用，请先启用父科室";
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

    // 获取所有需要启用的子科室ID
    std::vector<int64_t> allChildIds;
    for (int64_t parentId : parentIds)
    {
        auto childIds = deptDao.getChildDeptIds(parentId);
        allChildIds.insert(allChildIds.end(), childIds.begin(), childIds.end());
    }

    if (allChildIds.empty())
    {
        return true;  // 没有子科室，无需级联
    }

    // 级联启用所有子科室
    auto updateDto = DeptBatchUpdateDTO::createShared();
    updateDto->numIdList = allChildIds;
    updateDto->statusEnum = "ACTIVE";
    updateDto->updateBy = updateBy;
    updateDto->updateTime = updateTime;

    int affectRows = deptDao.batchUpdateStatus(allChildIds, updateDto);
    return affectRows == static_cast<int>(allChildIds.size());
}