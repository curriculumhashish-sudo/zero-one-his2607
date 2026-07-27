package com.zeroone.star.sysmgr.service.login;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.query.j1.login.DeptQuery;
import com.zeroone.star.project.vo.j1.login.DepartmentVO;
import com.zeroone.star.sysmgr.entity.login.WhaleOrganizationUnitDO;
import jakarta.validation.Valid;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public interface DeptService extends IService<WhaleOrganizationUnitDO> {
    List<DepartmentVO> deptList(@Valid DeptQuery query);
}
