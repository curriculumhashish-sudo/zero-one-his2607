#pragma once
#ifndef _DEPT_BATCH_STATUS_DAO_H_
#define _DEPT_BATCH_STATUS_DAO_H_

#include "BaseDAO.h"
#include "../../domain/dto/dept/DeptBatchStatusDTO.h"

class DeptBatchStatusDAO : public BaseDAO
{
public:
	// 删掉自定义构造、析构，完全继承BaseDAO默认构造
	~DeptBatchStatusDAO() override = default;


    // ========== 更新方法 ==========

    /**
     * 批量更新科室状态
     * @param numIdList 数字ID列表
     * @param updateDto 更新数据
     * @return 影响行数
     */
    int batchUpdateStatus(const std::vector<int64_t>& numIdList, const DeptBatchUpdateDTO::Wrapper& updateDto);

    // ========== 新增查询方法（父子状态校验用） ==========

    /**
     * 查询科室的父科室ID
     * @param deptId 科室ID
     * @return 父科室ID，0表示无父科室
     */
    int64_t getParentDeptId(int64_t deptId);

    /**
     * 检查科室是否有启用的子科室
     * @param deptId 科室ID
     * @return true-有启用子科室 false-无
     */
    bool hasActiveChildDept(int64_t deptId);

    /**
     * 查询科室的所有子科室ID列表
     * @param deptId 科室ID
     * @return 子科室ID列表
     */
    std::vector<int64_t> getChildDeptIds(int64_t deptId);

    /**
     * 查询科室是否启用
     * @param deptId 科室ID
     * @return true-启用 false-停用
     */
    bool isDeptActive(int64_t deptId);
};

#endif
