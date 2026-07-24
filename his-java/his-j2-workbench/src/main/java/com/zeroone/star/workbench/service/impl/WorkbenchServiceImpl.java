package com.zeroone.star.workbench.service.impl;

import com.zeroone.star.project.query.j2.workbench.UserWorkbenchQuery;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import com.zeroone.star.workbench.mapper.WhaleRolesMapper;
import com.zeroone.star.workbench.service.WorkbenchService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 工作台业务服务实现类
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Service
public class WorkbenchServiceImpl implements WorkbenchService {

    @Resource
    private WhaleRolesMapper whaleRolesMapper;

    @Override
    public List<UserWorkbenchVO> queryUserWorkbench(UserWorkbenchQuery query) {
       return null;
    }
}