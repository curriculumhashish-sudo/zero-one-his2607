package com.zeroone.star.sysmgr.service.role.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.util.IdUtil;
import cn.hutool.json.JSONObject;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.role.RoleAddDTO;
import com.zeroone.star.project.dto.j1.role.RoleModifyDTO;
import com.zeroone.star.project.query.j1.role.RoleQuery;
import com.zeroone.star.project.vo.j1.role.RoleNameVO;
import com.zeroone.star.project.vo.j1.role.RoleVO;
import com.zeroone.star.sysmgr.entity.role.RoleDO;
import com.zeroone.star.sysmgr.exception.RoleStatus;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.mapper.role.RoleClaimMapper;
import com.zeroone.star.sysmgr.mapper.role.RoleMapper;
import com.zeroone.star.sysmgr.service.role.IRoleService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import org.yaml.snakeyaml.util.UriEncoder;

import java.util.List;
import java.util.Locale;

/**
 * 角色管理业务服务实现
 * <p>
 * 实现角色的租户可见性、基本信息维护及安全删除规则
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Service
public class RoleServiceImpl extends ServiceImpl<RoleMapper, RoleDO> implements IRoleService {

    private static final String DEFAULT_TENANT_ID = "1";
    private static final String DEFAULT_OPERATOR_ID = "1";
    private static final String ROLE_NAME_PREFIX = "custom_role_";
    private static final String ROLE_CODE_PREFIX = "ROLE_CUSTOM_";
    private static final int ROLE_SUFFIX_LENGTH = 8;
    private static final int GENERATE_RETRY_LIMIT = 5;

    @Resource
    private RoleClaimMapper roleClaimMapper;

    @Resource
    private UserHolder userHolder;

    /**
     * 获取当前租户可见的角色名称列表。
     * <p>
     * 列表包含当前租户创建的角色和平台全局角色，仅返回下拉选择所需的编号与显示名称。
     * </p>
     *
     * @return 按显示名称和角色编号升序排列的角色名称列表
     */
    @Override
    public List<RoleNameVO> listRoleNames() {
        // 复用统一可见性条件，避免下拉列表越权展示其他租户的角色。
        LambdaQueryWrapper<RoleDO> wrapper = visibleRoleWrapper(getTenantId())
                .select(RoleDO::getId, RoleDO::getDisplayName)
                .orderByAsc(RoleDO::getDisplayName)
                .orderByAsc(RoleDO::getId);

        // 查询结果只转换为轻量级名称对象，不向调用方暴露角色完整信息。
        return baseMapper.selectList(wrapper).stream()
                .map(role -> BeanUtil.copyProperties(role, RoleNameVO.class))
                .toList();
    }

    /**
     * 按条件分页查询当前租户可见的角色。
     * <p>
     * 关键字同时匹配内部名称和显示名称，并支持按默认角色、内置角色状态筛选。
     * </p>
     *
     * @param query 分页与筛选条件
     * @return 角色分页数据
     */
    @Override
    public PageDTO<RoleVO> queryRoles(RoleQuery query) {
        // 查询范围固定为当前租户角色与平台全局角色。
        LambdaQueryWrapper<RoleDO> wrapper = visibleRoleWrapper(getTenantId());

        // 关键字为空时不拼接模糊查询条件，避免产生无意义的 LIKE 表达式。
        if (StringUtils.hasText(query.getKeyword())) {
            String keyword = query.getKeyword().trim();
            wrapper.and(condition -> condition
                    .like(RoleDO::getName, keyword)
                    .or()
                    .like(RoleDO::getDisplayName, keyword));
        }
        wrapper.eq(query.getIsDefault() != null, RoleDO::getIsDefault, query.getIsDefault())
                .eq(query.getIsStatic() != null, RoleDO::getIsStatic, query.getIsStatic())
                .orderByDesc(RoleDO::getCreatedAt)
                .orderByAsc(RoleDO::getId);

        // 使用项目统一的分页对象完成数据库分页和 VO 转换。
        Page<RoleDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        baseMapper.selectPage(page, wrapper);
        return PageDTO.create(page, RoleVO::new);
    }

    /**
     * 获取指定角色的详细信息。
     *
     * @param id 角色编号
     * @return 当前租户可见的角色详情
     * @throws SystemException 角色不存在或不在当前租户可见范围时抛出
     */
    @Override
    public RoleVO getRole(String id) {
        // 详情查询同样执行租户可见性校验，不能仅凭角色编号直接读取。
        RoleDO role = requireVisibleRole(id, getTenantId());
        return BeanUtil.copyProperties(role, RoleVO.class);
    }

    /**
     * 新增当前租户的自定义角色。
     * <p>
     * 内部名称和角色编码由服务端生成，调用方只负责提交可编辑的角色基本信息。
     * </p>
     *
     * @param dto 新增角色数据
     * @return 新增角色编号
     * @throws SystemException 唯一标识生成失败或角色写入失败时抛出
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public String addRole(RoleAddDTO dto) {
        // 为内部名称和角色编码生成同源随机后缀，避免由前端维护系统标识。
        String suffix = generateUniqueSuffix();

        // 新增角色固定属于当前租户，并明确标记为非内置、未删除角色。
        RoleDO role = new RoleDO();
        role.setName(ROLE_NAME_PREFIX + suffix);
        role.setCode(ROLE_CODE_PREFIX + suffix.toUpperCase(Locale.ROOT));
        role.setDisplayName(dto.getDisplayName().trim());
        role.setDescription(trimToNull(dto.getDescription()));
        role.setIsDefault(Boolean.TRUE.equals(dto.getIsDefault()));
        role.setIsStatic(false);
        role.setTenantId(getTenantId());
        role.setIsDeleted(0);

        // 插入结果或主键回填异常均视为创建失败，由事务统一回滚。
        if (baseMapper.insert(role) != 1 || !StringUtils.hasText(role.getId())) {
            throw new SystemException(RoleStatus.ROLE_CREATE_FAILED);
        }
        return role.getId();
    }

    /**
     * 修改角色的可编辑基本信息。
     * <p>
     * 仅更新显示名称、描述和默认角色标志，不修改内部名称、编码、内置标志、租户及授权关系。
     * 内置角色和自定义角色均可修改基本信息；普通内置角色的权限信息也允许通过角色授权流程维护，
     * admin 则仅允许修改基本信息，禁止变更其权限信息。
     * </p>
     *
     * @param dto 角色修改数据
     * @throws SystemException 角色不可见或更新失败时抛出
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void modifyRole(RoleModifyDTO dto) {
        String tenantId = getTenantId();

        // 修改前先确认角色对当前租户可见；内置角色与自定义角色均允许修改基本信息。
        RoleDO role = requireVisibleRole(dto.getId(), tenantId);

        // 未提交默认角色标志时保留数据库原值，避免局部修改覆盖已有状态。
        Boolean isDefault = dto.getIsDefault() == null ? role.getIsDefault() : dto.getIsDefault();

        // 使用定制 SQL 限定可修改字段，并允许将已有描述显式清空为 NULL。
        int updated = baseMapper.updateBasicInfo(
                role.getId(),
                tenantId,
                dto.getDisplayName().trim(),
                trimToNull(dto.getDescription()),
                isDefault,
                getCurrentUserId());
        if (updated != 1) {
            throw new SystemException(RoleStatus.ROLE_UPDATE_FAILED);
        }
    }

    /**
     * 逻辑删除当前租户的自定义角色及其授权记录。
     * <p>
     * 内置角色不可删除；仍被用户或职位引用的角色必须先解除关联。删除通过事务保证角色、
     * 角色声明和角色授权记录的状态一致性。
     * </p>
     *
     * @param id 角色编号
     * @throws SystemException 角色不可删除、仍被引用或逻辑删除失败时抛出
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeRole(String id) {
        String tenantId = getTenantId();
        RoleDO role = requireVisibleRole(id, tenantId);

        // 所有内置角色均不可删除；该限制不影响普通内置角色维护基本信息和权限信息。
        if (Boolean.TRUE.equals(role.getIsStatic())) {
            throw new SystemException(RoleStatus.BUILT_IN_ROLE_CANNOT_DELETE);
        }

        // 全局角色虽然对租户可见，但租户不能删除不属于自己的角色。
        if (!tenantId.equals(role.getTenantId())) {
            throw new SystemException(RoleStatus.ROLE_NOT_FOUND);
        }

        // 删除前必须解除用户和职位对角色的有效引用，避免产生失效关联。
        if (baseMapper.countActiveUserRoles(id) > 0) {
            throw new SystemException(RoleStatus.ROLE_ASSIGNED_TO_USER);
        }
        if (baseMapper.countActivePositionAssignments(id) > 0) {
            throw new SystemException(RoleStatus.ROLE_USED_BY_POSITION);
        }

        // 按“关联记录优先、角色主体最后”的顺序执行逻辑删除，任一步失败均回滚。
        String deletedBy = getCurrentUserId();
        roleClaimMapper.softDeleteByRoleId(id, deletedBy);
        baseMapper.softDeletePermissionGrants(id, deletedBy);
        if (baseMapper.softDeleteRole(id, tenantId, deletedBy) != 1) {
            throw new SystemException(RoleStatus.ROLE_DELETE_FAILED);
        }
    }

    /**
     * 构造角色租户可见性条件。
     *
     * @param tenantId 当前租户编号
     * @return 当前租户角色与全局角色的查询条件
     */
    private LambdaQueryWrapper<RoleDO> visibleRoleWrapper(String tenantId) {
        return Wrappers.<RoleDO>lambdaQuery()
                .and(condition -> condition
                        .eq(RoleDO::getTenantId, tenantId)
                        .or()
                        .isNull(RoleDO::getTenantId));
    }

    /**
     * 查询角色并校验其是否在当前租户的可见范围内。
     *
     * @param id       角色编号
     * @param tenantId 当前租户编号
     * @return 可见的角色实体
     * @throws SystemException 角色不存在或对当前租户不可见时抛出
     */
    private RoleDO requireVisibleRole(String id, String tenantId) {
        RoleDO role = baseMapper.selectOne(visibleRoleWrapper(tenantId)
                .eq(RoleDO::getId, id));
        if (role == null) {
            throw new SystemException(RoleStatus.ROLE_NOT_FOUND);
        }
        return role;
    }

    /**
     * 生成角色内部名称和编码共用的唯一后缀。
     *
     * @return 唯一随机后缀
     * @throws SystemException 达到重试上限后仍发生名称冲突时抛出
     */
    private String generateUniqueSuffix() {
        // 随机值发生冲突时有限重试，防止极端情况下无限循环。
        for (int i = 0; i < GENERATE_RETRY_LIMIT; i++) {
            String suffix = IdUtil.fastSimpleUUID().substring(0, ROLE_SUFFIX_LENGTH);
            if (baseMapper.countByName(ROLE_NAME_PREFIX + suffix) == 0) {
                return suffix;
            }
        }
        throw new SystemException(RoleStatus.ROLE_CREATE_FAILED);
    }

    /**
     * 从网关透传的用户请求头中读取当前租户编号。
     *
     * @return 当前租户编号；本地直连或请求头无效时返回开发环境默认租户
     */
    private String getTenantId() {
        ServletRequestAttributes attributes =
                (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();

        // 单元测试或本地直连可能没有完整网关上下文，使用项目约定的开发租户兜底。
        if (attributes == null) {
            return DEFAULT_TENANT_ID;
        }
        String userHeader = attributes.getRequest().getHeader("user");
        if (!StringUtils.hasText(userHeader)) {
            return DEFAULT_TENANT_ID;
        }
        try {
            // user 请求头经过 URL 编码，解码后再提取网关写入的 tenant_id。
            JSONObject userJson = new JSONObject(UriEncoder.decode(userHeader));
            String tenantId = userJson.getStr("tenant_id");
            return StringUtils.hasText(tenantId) ? tenantId : DEFAULT_TENANT_ID;
        } catch (Exception ignored) {
            return DEFAULT_TENANT_ID;
        }
    }

    /**
     * 获取当前登录用户编号，用于记录更新人与删除人。
     *
     * @return 当前用户编号；本地无认证上下文时返回默认操作人
     */
    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            if (user != null && StringUtils.hasText(user.getId())) {
                return user.getId();
            }
        } catch (Exception ignored) {
            // 本地直连调试没有认证上下文时，沿用公共字段填充器的默认操作人。
        }
        return DEFAULT_OPERATOR_ID;
    }

    /**
     * 清理可选文本字段首尾空白，并将空白字符串统一转换为 {@code null}。
     *
     * @param value 原始文本
     * @return 清理后的文本或 {@code null}
     */
    private String trimToNull(String value) {
        if (!StringUtils.hasText(value)) {
            return null;
        }
        return value.trim();
    }
}
