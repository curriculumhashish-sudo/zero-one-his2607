package com.zeroone.star.sysmgr.service.permission.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.permission.*;
import com.zeroone.star.project.query.j1.permission.PermissionQuery;
import com.zeroone.star.project.vo.j1.permission.*;
import com.zeroone.star.sysmgr.entity.permission.Permission;
import com.zeroone.star.sysmgr.mapper.permission.PermissionMapper;
import com.zeroone.star.sysmgr.service.permission.IPermissionService;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import jakarta.annotation.Resource;
import java.util.*;

@Service
public class PermissionServiceImpl
        extends ServiceImpl<PermissionMapper, Permission>
        implements IPermissionService {

    @Resource
    private PermissionMapper permissionMapper;

    @Override
    public List<PermissionTreeVO> listPermissionTree() {
        // TODO
        return Collections.emptyList();
    }

    @Override
    public PageDTO<PermissionVO> listPermissions(PermissionQuery query) {
        // TODO
        return new PageDTO<>();
    }

    @Override
    public PermissionVO createPermission(PermissionCreateDTO dto) {
        // TODO
        return new PermissionVO();
    }

    @Override
    public PermissionVO updatePermission(String id, PermissionUpdateDTO dto) {
        // TODO
        return new PermissionVO();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public DeleteResult deletePermission(String id) {
        // TODO
        return new DeleteResult(true, 0);
    }
}