package com.zeroone.star.j3.department.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentUpdateDTO;
import com.zeroone.star.project.query.j3.department.DepartmentQuery;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;
import com.zeroone.star.project.vo.j3.department.DepartmentVO;

import java.util.List;

/**
 * 描述：科室管理服务层接口
 * 版权：01星球
 * 地址：01星球总部
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
public interface DepartmentService {

    PageDTO<DepartmentVO> getDepartmentPage(DepartmentQuery query);

    /**
     * 获取科室树结构
     *
     * @param query 查询参数
     * @return 科室树VO列表
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
    DepartmentDetailVO getDepartmentDetail(String id);

    /**
     * 新增科室
     *
     * @param dto 创建科室DTO
     * @author 胡傲宇
     */
    void createDepartment(DepartmentCreateDTO dto) throws Exception;

    /**
     * 修改科室信息。
     *
     * @param id  科室ID
     * @param dto 修改内容
     */
    void updateDepartment(String id, DepartmentUpdateDTO dto);

    /**
     * 删除科室（逻辑删除）。
     *
     * @param id 科室ID
     */
    void deleteDepartment(String id);

    /**
     * 批量激活或停用科室
     *
     * @param dto 批量操作科室DTO
     * @author 胡傲宇
     */
    void activeOrInactiveDepartment(DepartmentBatchDTO dto);
}
