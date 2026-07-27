#pragma once
#ifndef _DEPT_BATCH_STATUS_SERVICE_H_
#define _DEPT_BATCH_STATUS_SERVICE_H_

#include "../../domain/dto/dept/DeptBatchStatusDTO.h"
#include "../../dao/dept/DeptBatchStatusDAO.h"
#include <string>
#include <vector>

/**
 * 批量操作结果
 */
struct BatchResult
{
    bool success;
    std::string errorMsg;
    int successCount;
    int failCount;

    BatchResult() : success(false), successCount(0), failCount(0) {}
};

class DeptBatchStatusService
{
public:
    /**
     * 批量启停科室业务逻辑入口
     * @param reqDto 前端提交请求DTO
     * @return 批量操作结果
     */
    BatchResult batchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& reqDto);

private:
    // ========== 父子状态校验方法 ==========

    /**
     * 校验父子科室状态规则
     * 规则1：存在启用子节点，不可停用父节点
     * 规则2：父节点未启用，子节点不可启用
     * 规则3：启用父节点后，自动级联启用所有子节点
     * @param deptIdList 待操作的科室ID列表
     * @param targetStatus 目标状态 0=停用 1=启用
     * @param errorMsg 错误信息
     * @return true-校验通过 false-校验失败
     */
    bool validateParentChildStatus(const std::vector<int64_t>& deptIdList, int targetStatus, std::string& errorMsg);

    /**
     * 级联启用子科室
     * @param parentIds 父科室ID列表
     * @param updateBy 操作人
     * @param updateTime 操作时间
     * @return true-成功 false-失败
     */
    bool cascadeEnableChildren(const std::vector<int64_t>& parentIds, const std::string& updateBy, const std::string& updateTime);
};

#endif
