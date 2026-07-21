package com.zeroone.star.project.service.impl;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.menu.RolePermissionAssignDTO;
import com.zeroone.star.project.entity.WhaleMenu;
import com.zeroone.star.project.entity.WhalePermissionGrant;
import com.zeroone.star.project.entity.WhaleRole;
import com.zeroone.star.project.mapper.MenuMapper;
import com.zeroone.star.project.mapper.PermissionGrantMapper;
import com.zeroone.star.project.mapper.RoleMapper;
import com.zeroone.star.project.service.IMenuService;
import com.zeroone.star.project.utils.tree.TreeNode;
import com.zeroone.star.project.utils.tree.TreeNodeMapper;
import com.zeroone.star.project.utils.tree.TreeUtils;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import jakarta.annotation.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.transaction.support.TransactionSynchronization;
import org.springframework.transaction.support.TransactionSynchronizationManager;
import org.springframework.web.client.RestTemplate;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

/**
 * 菜单服务实现
 * @author CatSN
 */
@Service
public class MenuServiceImpl implements IMenuService {

    private static final Logger log = LoggerFactory.getLogger(MenuServiceImpl.class);

    @Resource
    private MenuMapper menuMapper;

    @Resource
    private PermissionGrantMapper permissionGrantMapper;

    @Resource
    private RoleMapper roleMapper;

    @Resource
    private UserHolder userHolder;

    @Resource
    private RestTemplate restTemplate;

    @Value("${oauth2.service.name:his-oauth2}")
    private String oauth2ServiceName;

    @Value("${oauth2.context-path:/oauth2}")
    private String oauth2ContextPath;

    /**
     * WhaleMenu → MenuTreeVO 的树节点映射器
     * tnId/tnPid 用原始 String 保证树结构正确，
     * MenuTreeVO.id/pid 为 Integer，仅当 id 为纯数字时才填充，否则为 null
     */
    private static class WhaleMenuTreeNodeMapper implements TreeNodeMapper<WhaleMenu> {
        @Override
        public TreeNode objectMapper(WhaleMenu menu) {
            MenuTreeVO treeNode = new MenuTreeVO();
            // 设置树结构字段（String）
            treeNode.setTnId(menu.getId());
            if (menu.getParentId() == null || menu.getParentId().isEmpty()) {
                treeNode.setTnPid(null);
            } else {
                treeNode.setTnPid(menu.getParentId());
            }
            // 设置展示字段
            treeNode.setId(parseIntSafe(menu.getId()));
            treeNode.setPid(parseIntSafe(menu.getParentId()));
            treeNode.setText(menu.getDisplayName() != null ? menu.getDisplayName() : menu.getName());
            treeNode.setIcon(menu.getIcon());
            treeNode.setHref(menu.getPath());
            return treeNode;
        }

        private static Integer parseIntSafe(String value) {
            if (value == null || value.isEmpty()) {
                return null;
            }
            try {
                return Integer.parseInt(value);
            } catch (NumberFormatException e) {
                return null;
            }
        }
    }

    @Override
    public List<MenuTreeVO> listMenuTreeByLoginUser() {
        // 1. 获取当前登录用户
        UserDTO user;
        try {
            user = userHolder.getCurrentUser();
        } catch (Exception e) {
            log.warn("获取当前登录用户失败，返回空菜单列表", e);
            return Collections.emptyList();
        }
        if (user == null || user.getId() == null) {
            return Collections.emptyList();
        }

        // 2. 查询用户可见菜单
        List<WhaleMenu> menus = menuMapper.selectMenusByUserId(user.getId());
        if (menus == null || menus.isEmpty()) {
            return Collections.emptyList();
        }

        // 3. 拼树并返回
        return TreeUtils.listToTree(menus, new WhaleMenuTreeNodeMapper());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public boolean assignPermissions(RolePermissionAssignDTO dto) {
        if (dto == null || dto.getRoleId() == null) {
            return false;
        }

        // roleId(Long) → providerKey(String)
        String providerKey = String.valueOf(dto.getRoleId());

        // 0. 校验角色存在
        WhaleRole role = roleMapper.selectById(providerKey);
        if (role == null) {
            log.warn("角色不存在，roleId={}", providerKey);
            return false;
        }

        // 获取操作人ID（用于审计字段）
        String operatorId = null;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                operatorId = currentUser.getId();
            }
        } catch (Exception e) {
            log.warn("获取当前用户失败，审计字段将为空", e);
        }

        // 1. 删除旧授权（物理删除，全量替换语义）
        permissionGrantMapper.deleteByProviderKey(providerKey);

        // 2. 无新菜单则仅清理旧授权
        if (dto.getMenuIds() == null || dto.getMenuIds().isEmpty()) {
            registerCacheRefresh();
            return true;
        }

        // 3. menuIds(List<Long>) → List<String>，查 permission_name
        List<String> menuIdStrings = dto.getMenuIds().stream()
                .map(String::valueOf)
                .collect(Collectors.toList());
        List<String> permissionNames = permissionGrantMapper.selectPermissionNamesByMenuIds(menuIdStrings);

        if (permissionNames == null || permissionNames.isEmpty()) {
            log.warn("所选菜单均未关联权限，roleId={}，menuIds={}", providerKey, menuIdStrings);
            registerCacheRefresh();
            return true;
        }

        // 4. 批量插入新授权
        LocalDateTime now = LocalDateTime.now();
        List<WhalePermissionGrant> grants = new ArrayList<>();
        for (String pn : permissionNames) {
            WhalePermissionGrant grant = new WhalePermissionGrant();
            grant.setId(UUID.randomUUID().toString());
            grant.setPermissionName(pn);
            grant.setProviderName("R");
            grant.setProviderKey(providerKey);
            grant.setCreatedAt(now);
            grant.setCreatedBy(operatorId);
            grant.setIsDeleted(0);
            grants.add(grant);
        }
        permissionGrantMapper.batchInsertGrants(grants);

        // 5. 事务提交后刷新 OAuth2 缓存
        registerCacheRefresh();
        return true;
    }

    /**
     * 注册事务提交后的缓存刷新回调；
     * 若无事务上下文则直接刷新
     */
    private void registerCacheRefresh() {
        if (TransactionSynchronizationManager.isSynchronizationActive()) {
            TransactionSynchronizationManager.registerSynchronization(new TransactionSynchronization() {
                @Override
                public void afterCommit() {
                    refreshOauth2Cache();
                }
            });
        } else {
            refreshOauth2Cache();
        }
    }

    /**
     * 调用 his-oauth2 的缓存刷新接口，重建 Redis 中的 URL→角色 列表映射。
     * 失败仅记录日志，不回滚已提交的权限变更。
     */
    private void refreshOauth2Cache() {
        try {
            String url = String.format("http://%s%s/open/resources/cache/reload",
                    oauth2ServiceName, oauth2ContextPath);
            restTemplate.put(url, null);
            log.info("OAuth2 资源缓存刷新成功: {}", url);
        } catch (Exception e) {
            log.warn("OAuth2 资源缓存刷新失败，权限变更已生效但缓存可能延迟", e);
        }
    }
}
