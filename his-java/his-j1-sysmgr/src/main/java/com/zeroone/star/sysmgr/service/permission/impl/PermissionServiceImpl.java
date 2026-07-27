package com.zeroone.star.sysmgr.service.permission.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.json.JSONObject;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.permission.DeleteResult;
import com.zeroone.star.project.dto.j1.permission.PermissionCreateDTO;
import com.zeroone.star.project.dto.j1.permission.PermissionUpdateDTO;
import com.zeroone.star.project.query.j1.permission.PermissionQuery;
import com.zeroone.star.project.utils.tree.TreeUtils;
import com.zeroone.star.project.vo.j1.permission.PermissionTreeVO;
import com.zeroone.star.project.vo.j1.permission.PermissionVO;
import com.zeroone.star.sysmgr.entity.permission.Permission;
import com.zeroone.star.sysmgr.exception.PermissionStatus;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.mapper.permission.PermissionMapper;
import com.zeroone.star.sysmgr.service.permission.IPermissionService;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

import java.util.ArrayList;
import java.util.List;

@Slf4j
@Service
public class PermissionServiceImpl
        extends ServiceImpl<PermissionMapper, Permission>
        implements IPermissionService {

    @Resource
    private PermissionMapper permissionMapper;

    @Resource
    private UserHolder userHolder;

    private final RestTemplate restTemplate = new RestTemplate();

    @Value("${his.cache.reload-url:http://localhost:10400/open/resources/cache/reload}")
    private String cacheReloadUrl;

    // ======================== 接口 1：权限名称树 ========================

    @Override
    public List<PermissionTreeVO> listPermissionTree() {
        String tenantId = getTenantId();
        List<Permission> permissions = baseMapper.selectList(
                new QueryWrapper<Permission>()
                        .eq("tenant_id", tenantId)
                        .orderByAsc("display_name"));
        return TreeUtils.listToTree(permissions, p -> {
            PermissionTreeVO node = new PermissionTreeVO();
            node.setTnId(p.getName());
            node.setTnPid(p.getParentName());
            node.setDisplayName(p.getDisplayName());
            return node;
        });
    }

    // ======================== 接口 2：条件分页查询 ========================

    @Override
    public PageDTO<PermissionVO> listPermissions(PermissionQuery query) {
        String tenantId = getTenantId();
        QueryWrapper<Permission> wrapper = new QueryWrapper<>();
        wrapper.eq("tenant_id", tenantId);
        if (StringUtils.hasText(query.getKeyword())) {
            wrapper.and(w -> w.like("name", query.getKeyword())
                    .or().like("display_name", query.getKeyword()));
        }
        wrapper.orderByAsc("display_name");
        Page<Permission> page = new Page<>(query.getPageIndex(), query.getPageSize());
        baseMapper.selectPage(page, wrapper);
        return PageDTO.create(page, PermissionVO::new);
    }

    // ======================== 接口 3：新增权限 ========================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public PermissionVO createPermission(PermissionCreateDTO dto) {
        String tenantId = getTenantId();
        if (permissionMapper.countByName(dto.getName(), tenantId) > 0) {
            throw new SystemException(PermissionStatus.PERMISSION_EXISTS);
        }
        if (StringUtils.hasText(dto.getParentName())
                && !validateParentExists(dto.getParentName(), tenantId)) {
            throw new SystemException(PermissionStatus.PARENT_NOT_EXISTS);
        }
        Permission entity = new Permission();
        entity.setName(dto.getName());
        entity.setDisplayName(dto.getDisplayName());
        entity.setParentName(emptyToNull(dto.getParentName()));
        entity.setDescription(dto.getDescription());
        entity.setTenantId(tenantId);
        entity.setIsDeleted(0);
        baseMapper.insert(entity);
        refreshCache();
        return BeanUtil.copyProperties(entity, PermissionVO.class);
    }

    // ======================== 接口 4：编辑权限 ========================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public PermissionVO updatePermission(String id, PermissionUpdateDTO dto) {
        String tenantId = getTenantId();
        Permission existing = selectByIdAndTenant(id, tenantId);
        if (existing == null) {
            throw new SystemException(PermissionStatus.PERMISSION_NOT_EXISTS);
        }
        if (StringUtils.hasText(dto.getParentName())
                && !dto.getParentName().equals(existing.getParentName())) {
            validateParentChange(existing, dto.getParentName(), tenantId);
        }
        existing.setDisplayName(dto.getDisplayName());
        existing.setParentName(emptyToNull(dto.getParentName()));
        existing.setDescription(dto.getDescription());
        baseMapper.updateById(existing);
        refreshCache();
        return BeanUtil.copyProperties(existing, PermissionVO.class);
    }

    private void validateParentChange(Permission existing, String newParentName, String tenantId) {
        if (newParentName.equals(existing.getName())) {
            throw new SystemException(PermissionStatus.SELF_PARENT);
        }
        if (!validateParentExists(newParentName, tenantId)) {
            throw new SystemException(PermissionStatus.PARENT_NOT_EXISTS);
        }
        String ancestor = permissionMapper.selectParentName(newParentName, tenantId);
        while (ancestor != null) {
            if (ancestor.equals(existing.getName())) {
                throw new SystemException(PermissionStatus.DESCENDANT_PARENT);
            }
            ancestor = permissionMapper.selectParentName(ancestor, tenantId);
        }
    }

    // ======================== 接口 5：级联删除权限 ========================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public DeleteResult deletePermission(String id) {
        String tenantId = getTenantId();
        Permission permission = selectByIdAndTenant(id, tenantId);
        if (permission == null) {
            throw new SystemException(PermissionStatus.PERMISSION_NOT_EXISTS);
        }
        List<String> namesToDelete = new ArrayList<>();
        namesToDelete.add(permission.getName());
        collectChildNames(permission.getName(), tenantId, namesToDelete);
        String deletedBy = getCurrentUserId();
        int permCount = permissionMapper.batchDeletePermissions(namesToDelete, deletedBy, tenantId);
        permissionMapper.batchDeleteGrants(namesToDelete, deletedBy, tenantId);
        refreshCache();
        return new DeleteResult(true, permCount);
    }

    private void collectChildNames(String parentName, String tenantId, List<String> result) {
        List<String> children = permissionMapper.selectChildNames(parentName, tenantId);
        for (String child : children) {
            result.add(child);
            collectChildNames(child, tenantId, result);
        }
    }

    // ======================== 私有工具方法 ========================

    private Permission selectByIdAndTenant(String id, String tenantId) {
        return baseMapper.selectOne(
                new QueryWrapper<Permission>()
                        .eq("id", id)
                        .eq("tenant_id", tenantId));
    }

    private boolean validateParentExists(String parentName, String tenantId) {
        return baseMapper.selectCount(
                new QueryWrapper<Permission>()
                        .eq("name", parentName)
                        .eq("tenant_id", tenantId)) > 0;
    }

    private String getTenantId() {
        ServletRequestAttributes attrs =
                (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
        if (attrs == null) {
            return "1";
        }
        String userStr = attrs.getRequest().getHeader("user");
        if (!StringUtils.hasText(userStr)) {
            return "1";
        }
        JSONObject userJson = new JSONObject(userStr);
        String tenantId = userJson.getStr("tenant_id");
        if (!StringUtils.hasText(tenantId)) {
            return "1";
        }
        return tenantId;
    }

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user.getId();
        } catch (Exception e) {
            throw new SystemException(PermissionStatus.GET_USER_FAILED);
        }
    }

    private String emptyToNull(String value) {
        if (!StringUtils.hasText(value)) {
            return null;
        }
        return value;
    }

    private void refreshCache() {
        new Thread(() -> {
            try {
                restTemplate.put(cacheReloadUrl, null);
                log.info("权限缓存已刷新");
            } catch (Exception e) {
                log.error("刷新权限缓存失败", e);
            }
        }, "cache-reload").start();
    }
}
