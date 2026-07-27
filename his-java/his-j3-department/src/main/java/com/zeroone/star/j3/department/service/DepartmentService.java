package com.zeroone.star.j3.department.service;

import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;

import java.util.List;

/**
 * <p>
 * 描述：科室管理服务层接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
public interface DepartmentService {
    /**
     * 获取科室名称树（支持门诊/住院筛选）
     *
     * @param query 查询参数
     * @return 科室树列表
     * @author 胡傲宇
     */
    List<DepartmentTreeVO> getDepartmentTree(DepartmentTreeQuery query);

    /**
     * 获取科室详情
     *
     * @param id 科室ID
     * @return 科室详情VO
     * @author 胡傲宇
     */
    DepartmentDetailVO getDepartmentDetail(Long id);

    /**
     * 新增科室
     *
     * @param dto 创建科室DTO
     * @author 胡傲宇
     */
    void createDepartment(DepartmentCreateDTO dto) throws Exception;

    /**
     * 启用/停用科室(支持批量)
     *
     * @param dto 批量操作科室状态DTO
     * @author 胡傲宇
     */
    void activeOrInactiveDepartment(DepartmentBatchDTO dto);
}
