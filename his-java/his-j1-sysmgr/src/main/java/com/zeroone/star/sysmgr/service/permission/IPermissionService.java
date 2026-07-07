package com.zeroone.star.sysmgr.service.permission;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.permission.*;
import com.zeroone.star.project.query.permission.PermissionQuery;
import com.zeroone.star.project.vo.permission.*;
import com.zeroone.star.sysmgr.entity.Permission;
import java.util.List;

public interface IPermissionService extends IService<Permission> {
    List<PermissionTreeVO> listPermissionTree();
    PageDTO<PermissionVO> listPermissions(PermissionQuery query);
    PermissionVO createPermission(PermissionCreateDTO dto);
    PermissionVO updatePermission(String id, PermissionUpdateDTO dto);
    DeleteResult deletePermission(String id);
}