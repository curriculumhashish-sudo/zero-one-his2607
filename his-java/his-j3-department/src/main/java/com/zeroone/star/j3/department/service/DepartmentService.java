package com.zeroone.star.j3.department.service;

import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;

import java.util.List;

public interface DepartmentService {

    List<DepartmentTreeVO> getDepartmentTree(DepartmentTreeQuery query);

    DepartmentDetailVO getDepartmentDetail(Long id);

    void createDepartment(DepartmentCreateDTO dto) throws Exception;

    void activeOrInactiveDepartment(DepartmentBatchDTO dto);
}
