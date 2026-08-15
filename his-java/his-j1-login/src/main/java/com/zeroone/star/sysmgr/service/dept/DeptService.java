package com.zeroone.star.sysmgr.service.dept;

import com.zeroone.star.project.dto.j1.login.DeptSelectDTO;
import com.zeroone.star.project.query.j1.login.DeptQuery;
import com.zeroone.star.project.vo.j1.login.DepartmentVO;
import com.zeroone.star.project.vo.j1.login.DeptBindResultVO;
import jakarta.validation.Valid;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public interface DeptService {
    List<DepartmentVO> deptList(@Valid DeptQuery query);

    DeptBindResultVO selectDept(@Valid DeptSelectDTO dto);
}
