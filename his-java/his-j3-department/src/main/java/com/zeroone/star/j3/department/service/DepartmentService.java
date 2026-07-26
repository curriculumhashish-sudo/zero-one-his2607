package com.zeroone.star.j3.department.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.query.j3.department.DepartmentQuery;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;
import com.zeroone.star.project.vo.j3.department.DepartmentVO;

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

    PageDTO<DepartmentVO> getDepartmentPage(DepartmentQuery query);

    List<DepartmentTreeVO> getDepartmentTree(DepartmentTreeQuery query);

    DepartmentDetailVO getDepartmentDetail(Long id);

    void createDepartment(DepartmentCreateDTO dto) throws Exception;

    void activeOrInactiveDepartment(DepartmentBatchDTO dto);
}
