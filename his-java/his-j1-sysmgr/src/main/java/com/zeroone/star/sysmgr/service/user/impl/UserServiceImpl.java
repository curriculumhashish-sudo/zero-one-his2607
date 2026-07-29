package com.zeroone.star.sysmgr.service.user.impl;

import cn.hutool.crypto.digest.BCrypt;
import cn.hutool.json.JSONObject;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.dto.j1.user.UserUpdateDTO;
import com.zeroone.star.project.query.j1.user.UserListQuery;
import com.zeroone.star.project.query.j1.user.UserQuery;
import com.zeroone.star.project.vo.j1.user.UserDetailVO;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.project.vo.j1.user.UserPageVO;
import com.zeroone.star.sysmgr.entity.resource.AdmHealthcareService;
import com.zeroone.star.sysmgr.entity.resource.AdmOrganization;
import com.zeroone.star.sysmgr.entity.role.RoleDO;
import com.zeroone.star.sysmgr.entity.user.*;
import com.zeroone.star.project.exception.SystemException;
import com.zeroone.star.sysmgr.exception.UserStatus;
import com.zeroone.star.sysmgr.mapper.resource.AdmHealthcareServiceMapper;
import com.zeroone.star.sysmgr.mapper.resource.AdmLocationMapper;
import com.zeroone.star.sysmgr.mapper.resource.AdmOrganizationMapper;
import com.zeroone.star.sysmgr.mapper.role.RoleMapper;
import com.zeroone.star.sysmgr.mapper.user.*;
import com.zeroone.star.sysmgr.service.user.*;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;

/**
 * <p>
 * 系统用户 服务实现类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Service
public class UserServiceImpl extends ServiceImpl<UserMapper, User> implements IUserService {

    @Resource
    private UserHolder userHolder;
    @Resource
    private IUserRoleService userRoleService;
    @Resource
    private PractitionerMapper practitionerMapper;
    @Resource
    private PrAssignMapper prAssignMapper;
    @Resource
    private PrAssignServiceMapper prAssignServiceMapper;
    @Resource
    private AdmOrganizationMapper orgMapper;
    @Resource
    private AdmLocationMapper locationMapper;
    @Resource
    private AdmHealthcareServiceMapper healthcareServiceMapper;
    @Resource
    private RoleMapper roleMapper;
    @Resource
    private UserOrganizationUnitMapper userOrganizationUnitMapper;
    @Resource
    private UserTenantMapper userTenantMapper;
    @Resource
    private UserRoleMapper userRoleMapper;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public boolean softDeleteById(String id) {
        // 1. 检查用户是否存在（自动过滤 is_deleted=0）
        User user = baseMapper.selectById(id);
        if (user == null) {
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }

        String deletedBy = getCurrentUserId();
        LocalDateTime deletedAt = LocalDateTime.now();

        // ========== 2. 软删除用户主表 ==========
        // 2.1 先更新删除信息
        User userUpdate = new User();
        userUpdate.setId(id);
        userUpdate.setDeletedBy(deletedBy);
        userUpdate.setDeletedAt(deletedAt);
        baseMapper.updateById(userUpdate);   // 只更新 deleted_by 和 deleted_at，is_deleted 仍为 0
        // 2.2 再调用逻辑删除（将 is_deleted 置为 1）
        baseMapper.deleteById(id);

        // ========== 3. 软删除用户角色关联 ==========
        // 查询该用户的所有角色关联（未删除的）
        List<UserRole> userRoles = userRoleMapper.selectList(
                new LambdaQueryWrapper<UserRole>()
                        .eq(UserRole::getUserId, id)
                        .eq(UserRole::getIsDeleted, 0)
        );
        if (!userRoles.isEmpty()) {
            // 批量更新删除信息
            for (UserRole ur : userRoles) {
                ur.setDeletedBy(deletedBy);
                ur.setDeletedAt(deletedAt);
                userRoleMapper.updateById(ur);
            }
            // 批量逻辑删除
            List<String> roleIds = userRoles.stream().map(UserRole::getId).collect(Collectors.toList());
            userRoleMapper.deleteBatchIds(roleIds);
        }

        // ========== 4. 处理 practitioner 及相关联表 ==========
        Practitioner practitioner = practitionerMapper.selectOne(
                new LambdaQueryWrapper<Practitioner>()
                        .eq(Practitioner::getUserId, id)
                        .eq(Practitioner::getIsDeleted, 0)
        );
        if (practitioner != null) {
            String practitionerId = practitioner.getId();

            // 4.1 软删除 practitioner
            Practitioner practitionerUpdate = new Practitioner();
            practitionerUpdate.setId(practitionerId);
            practitionerUpdate.setDeletedBy(deletedBy);
            practitionerUpdate.setDeletedAt(deletedAt);
            practitionerMapper.updateById(practitionerUpdate);
            practitionerMapper.deleteById(practitionerId);

            // 4.2 查询该 practitioner 下的所有 pr_assign（未删除的）
            List<PrAssign> prAssigns = prAssignMapper.selectList(
                    new LambdaQueryWrapper<PrAssign>()
                            .eq(PrAssign::getPractitionerId, practitionerId)
                            .eq(PrAssign::getIsDeleted, 0)
            );
            if (!prAssigns.isEmpty()) {
                // 收集 assignment_id 列表（用于删除 pr_assign_service）
                List<String> assignIds = prAssigns.stream().map(PrAssign::getId).collect(Collectors.toList());

                // 4.2.1 先删子表 pr_assign_service
                // 查询所有关联的 pr_assign_service（未删除的）
                List<PrAssignService> services = prAssignServiceMapper.selectList(
                        new LambdaQueryWrapper<PrAssignService>()
                                .in(PrAssignService::getAssignmentId, assignIds)
                                .eq(PrAssignService::getIsDeleted, 0)
                );
                if (!services.isEmpty()) {
                    for (PrAssignService service : services) {
                        service.setDeletedBy(deletedBy);
                        service.setDeletedAt(deletedAt);
                        prAssignServiceMapper.updateById(service);
                    }
                    List<String> serviceIds = services.stream().map(PrAssignService::getId).collect(Collectors.toList());
                    prAssignServiceMapper.deleteBatchIds(serviceIds);
                }

                // 4.2.2 再删父表 pr_assign
                for (PrAssign pa : prAssigns) {
                    pa.setDeletedBy(deletedBy);
                    pa.setDeletedAt(deletedAt);
                    prAssignMapper.updateById(pa);
                }
                prAssignMapper.deleteBatchIds(assignIds);
            }
        }

        // ========== 5. 软删除用户-组织单元关联 ==========
        List<UserOrganizationUnit> units = userOrganizationUnitMapper.selectList(
                new LambdaQueryWrapper<UserOrganizationUnit>()
                        .eq(UserOrganizationUnit::getUserId, id)
                        .eq(UserOrganizationUnit::getIsDeleted, 0)
        );
        if (!units.isEmpty()) {
            for (UserOrganizationUnit unit : units) {
                unit.setDeletedBy(deletedBy);
                unit.setDeletedAt(deletedAt);
                userOrganizationUnitMapper.updateById(unit);
            }
            List<String> unitIds = units.stream().map(UserOrganizationUnit::getId).collect(Collectors.toList());
            userOrganizationUnitMapper.deleteBatchIds(unitIds);
        }

        // ========== 6. 软删除用户-租户关联 ==========
        List<UserTenant> tenants = userTenantMapper.selectList(
                new LambdaQueryWrapper<UserTenant>()
                        .eq(UserTenant::getUserId, id)
                        .eq(UserTenant::getIsDeleted, 0)
        );
        if (!tenants.isEmpty()) {
            for (UserTenant tenant : tenants) {
                tenant.setDeletedBy(deletedBy);
                tenant.setDeletedAt(deletedAt);
                userTenantMapper.updateById(tenant);
            }
            List<String> tenantIds = tenants.stream().map(UserTenant::getId).collect(Collectors.toList());
            userTenantMapper.deleteBatchIds(tenantIds);
        }
        return true;
    }

    @Override
    public boolean updateStatus(String id, Integer isActive) {
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        return baseMapper.updateStatus(id, isActive) > 0;
    }

    @Override
    public boolean lockUser(String id, Integer isLockedOut) {
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        return baseMapper.lockUser(id, isLockedOut) > 0;
    }

    @Override
    public boolean updatePassword(String id, String newPassword) {
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        String hashed = BCrypt.hashpw(newPassword);
        return baseMapper.updatePassword(id, hashed) > 0;
    }

    @Override
    public PageDTO<UserListVO> getList(UserListQuery query) {
        QueryWrapper<User> wrapper = new QueryWrapper<>();

        if (StringUtils.hasText(query.getKeyword())) {
            wrapper.and(
                    w -> w.like("username", query.getKeyword())
                            .or().like("name", query.getKeyword())
                            .or().like("email", query.getKeyword()));
        }

        if (query.getIsActive() != null) {
            wrapper.eq("isActive", query.getIsActive());
        }


        Page<User> page = new Page<>(query.getPageIndex(), query.getPageSize());
        baseMapper.selectPage(page, wrapper);

        return PageDTO.create(page, user -> {
            UserListVO vo = new UserListVO();
            BeanUtils.copyProperties(user, vo);
            return vo;
        });
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean saveUser(UserAddDTO userAddDTO) {
        // 1.校验用户名唯一
        if (baseMapper.selectCount(new LambdaQueryWrapper<User>().eq(User::getUserName, userAddDTO.getUsername())) > 0) {
            throw new SystemException(UserStatus.USERNAME_DUPLICATE);
        }
        // 2.校验邮箱唯一
        if (StringUtils.hasText(userAddDTO.getEmail())) {
            if (baseMapper.selectCount(new LambdaQueryWrapper<User>().eq(User::getEmail, userAddDTO.getEmail())) > 0) {
                throw new SystemException(UserStatus.EMAIL_DUPLICATE);
            }
        }
        // 3.插入 whale_users
        User user = new User();
        user.setUserName(userAddDTO.getUsername());
        user.setEmail(userAddDTO.getEmail());
        user.setPhoneNumber(userAddDTO.getPhoneNumber());
        user.setName(userAddDTO.getName());
        user.setPasswordHash(BCrypt.hashpw(userAddDTO.getPassword()));
        user.setIsActive(userAddDTO.getIsActive() != null ? userAddDTO.getIsActive() == 1 : null);
        baseMapper.insert(user);
        String userId = user.getId();

        String defaultDeptId = "";
        if(StringUtils.hasText(userAddDTO.getDefaultOrgId())) {
            defaultDeptId=userAddDTO.getDefaultOrgId();
        }else {
            String currentUserId = getCurrentUserId();
            Practitioner currentPractitioner = practitionerMapper.selectOne(
                    new LambdaQueryWrapper<Practitioner>().eq(Practitioner::getUserId, currentUserId));
            if (currentPractitioner.getOrgId() != null) {
                defaultDeptId = currentPractitioner.getOrgId();
            }
        }

        // 4.插入 adm_practitioner
        Practitioner practitioner = new Practitioner();
        practitioner.setStatusEnum("ACTIVE");
        practitioner.setUserId(userId);
        practitioner.setName(userAddDTO.getName());
        practitioner.setPhone(userAddDTO.getPhoneNumber());
        practitioner.setGender(userAddDTO.getGenderEnum());
        practitioner.setBirthDate(userAddDTO.getBirthDate());
        practitioner.setAddressStreet(userAddDTO.getAddress());
        practitioner.setYbNo(userAddDTO.getYbNo());
        practitioner.setSignature(userAddDTO.getSignature());
        practitioner.setDrProfttlCode(StringUtils.hasText(userAddDTO.getDrProfttlCode()) ? Integer.valueOf(userAddDTO.getDrProfttlCode()) : null);
        practitioner.setPharPracCertNo(userAddDTO.getPharPracCertNo());
        practitioner.setOrgId(defaultDeptId);
        practitionerMapper.insert(practitioner);

        // 5.插入 whale_user_roles
        if (userAddDTO.getRoleIds() != null) {
            for (String roleId : userAddDTO.getRoleIds()) {
                UserRole ur = new UserRole();
                ur.setUserId(userId);
                ur.setRoleId(roleId);
                userRoleService.save(ur);
            }
        }
        // 6.查询角色 code 映射，按角色插入 adm_pr_assign
        Map<String, String> codeMap = userAddDTO.getRoleIds() == null || userAddDTO.getRoleIds().isEmpty()
                ? Collections.emptyMap()
                : roleMapper.selectBatchIds(userAddDTO.getRoleIds()).stream()
                        .filter(r -> r.getIsDeleted() == 0)
                        .collect(Collectors.toMap(RoleDO::getId, r -> r.getCode() != null ? r.getCode() : ""));
        if (!codeMap.isEmpty()) {
            // 医生 → 出诊科室 + 号源
            if (codeMap.containsValue("ROLE_DOCTOR")) {
                String roleId = getKeyByValue(codeMap, "ROLE_DOCTOR");
                if (userAddDTO.getConsultDeptIds() != null) {
                    for (String orgId : userAddDTO.getConsultDeptIds()) {
                        prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, orgId, null));
                    }
                }
                if (userAddDTO.getReceptionSourceIds() != null) {
                    String orgId = (userAddDTO.getConsultDeptIds() != null && !userAddDTO.getConsultDeptIds().isEmpty())
                            ? userAddDTO.getConsultDeptIds().get(0)
                            : null;
                    for (String svcId : userAddDTO.getReceptionSourceIds()) {
                        PrAssign pa = newPrAssign(practitioner.getId(), roleId, orgId, null);
                        prAssignMapper.insert(pa);
                        PrAssignService pas = new PrAssignService();
                        pas.setAssignmentId(pa.getId());
                        pas.setHealthcareServiceId(svcId);
                        prAssignServiceMapper.insert(pas);
                    }
                }
            }
            // 护士 → 病区
            if (codeMap.containsValue("ROLE_NURSE") && userAddDTO.getWardIds() != null) {
                String roleId = getKeyByValue(codeMap, "ROLE_NURSE");
                for (String locId : userAddDTO.getWardIds()) {
                    prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                }
            }
            // 药师 → 药房
            if (codeMap.containsValue("ROLE_PHARMACIST") && userAddDTO.getPharmacyIds() != null) {
                String roleId = getKeyByValue(codeMap, "ROLE_PHARMACIST");
                for (String locId : userAddDTO.getPharmacyIds()) {
                    prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                }
            }
            // 库管 → 库房 + 药房
            if (codeMap.containsValue("ROLE_WAREHOUSE")) {
                String roleId = getKeyByValue(codeMap, "ROLE_WAREHOUSE");
                if (userAddDTO.getWarehouseIds() != null) {
                    for (String locId : userAddDTO.getWarehouseIds()) {
                        prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                    }
                }
                if (userAddDTO.getPharmacyIds() != null) {
                    for (String locId : userAddDTO.getPharmacyIds()) {
                        prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                    }
                }
            }
        }
        // 7.关联科室
        UserOrganizationUnit unit = new UserOrganizationUnit();
        unit.setUserId(userId);
        unit.setOrganizationUnitId(defaultDeptId);
        userOrganizationUnitMapper.insert(unit);

        // 8.关联租户
        UserTenant userTenant = new UserTenant();
        userTenant.setUserId(userId);
        userTenant.setTenantId(getTenantId());
        userTenantMapper.insert(userTenant);
        return true;
    }

    @Override
    public PageDTO<UserPageVO> getPage(UserQuery query) {
        // 1.分页参数默认值
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        long pageSize  = query.getPageSize()  > 0 ? query.getPageSize()  : 10;

        // 2.构建查询条件：查询 whale_users 表（is_deleted 由 @TableLogic 自动过滤）
        QueryWrapper<User> wrapper = new QueryWrapper<>();
        wrapper.eq("tenant_id", getTenantId()); // 租户过滤
        if (StringUtils.hasText(query.getKeyword())) {
            wrapper.and(w -> w                                  // 关键词：用户名 或 姓名
                    .like("user_name", query.getKeyword())
                    .or().like("name", query.getKeyword()));
        }
        if (query.getIsActive() != null) {
            wrapper.eq("is_active", query.getIsActive());       // 启用/停用筛选
        }
        if (query.getIsLockedOut() != null) {
            wrapper.eq("is_locked_out", query.getIsLockedOut());// 锁定/未锁定筛选
        }

        // 3.分页查询
        Page<User> page = new Page<>(pageIndex, pageSize);
        baseMapper.selectPage(page, wrapper);

        // 4.批量查询这些用户拥有的角色（关联 whale_user_roles + whale_roles）
        List<String> userIds = page.getRecords().stream().map(User::getId).collect(Collectors.toList());
        Map<String, List<IUserRoleService.RoleInfo>> roleMap = userIds.isEmpty()
                ? Collections.emptyMap()
                : userRoleService.getRoleInfoByUserIds(userIds);

        // 5.组装返回：User → UserPageVO，补充角色ID和角色名
        return PageDTO.create(page, user -> {
            UserPageVO vo = new UserPageVO();
            vo.setId(user.getId());
            vo.setUsername(user.getUserName());
            vo.setName(user.getName());
            vo.setPhoneNumber(user.getPhoneNumber());
            vo.setEmail(user.getEmail());
            // Boolean → Integer
            vo.setIsActive(user.getIsActive() != null ? (user.getIsActive() ? 1 : 0) : null);
            vo.setIsLockedOut(user.getIsLockedOut() != null ? (user.getIsLockedOut() ? 1 : 0) : null);
            vo.setAccessFailedCount(user.getAccessFailedCount());
            vo.setLockoutEnd(user.getLockoutEnd());
            vo.setCreatedAt(user.getCreatedAt());
            vo.setUpdatedAt(user.getUpdatedAt());
            List<IUserRoleService.RoleInfo> roles = roleMap.getOrDefault(user.getId(), Collections.emptyList());
            vo.setRoleIds(roles.stream().map(IUserRoleService.RoleInfo::getRoleId).collect(Collectors.toList()));
            vo.setRoleNames(roles.stream().map(IUserRoleService.RoleInfo::getRoleName).collect(Collectors.toList()));
            return vo;
        });
    }

    @Override
    public UserDetailVO getById(String id) {
        // 1.查用户主表
        User user = baseMapper.selectById(id);
        if (user == null) {
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        // 2.查医务人员详情（通过 user_id 关联）
        Practitioner practitioner = practitionerMapper.selectOne(
                new LambdaQueryWrapper<Practitioner>().eq(Practitioner::getUserId, id));
        // 3.查角色
        List<UserRole> userRoles = userRoleService.list(
                new LambdaQueryWrapper<UserRole>().eq(UserRole::getUserId, id));
        Set<String> roleIds = userRoles.stream().map(UserRole::getRoleId).collect(Collectors.toSet());
        List<RoleDO> roles = roleIds.isEmpty() ? Collections.emptyList()
                : roleMapper.selectBatchIds(roleIds).stream().filter(r -> r.getIsDeleted() == 0).toList();
        // 4.默认科室名称
        String defaultDeptName = "";
        if (practitioner != null && practitioner.getOrgId() != null) {
            AdmOrganization org = orgMapper.selectById(practitioner.getOrgId());
            if (org != null) defaultDeptName = org.getName();
        }
        // 5.组装返回
        UserDetailVO vo = new UserDetailVO();
        vo.setId(user.getId());
        vo.setUsername(user.getUserName());
        vo.setName(user.getName());
        vo.setEmail(user.getEmail());
        vo.setRoleIds(new ArrayList<>(roleIds));
        vo.setRoleNames(roles.stream().map(RoleDO::getDisplayName).collect(Collectors.toList()));
        vo.setIsActive(user.getIsActive() != null ? (user.getIsActive() ? 1 : 0) : null);
        vo.setIsLockedOut(user.getIsLockedOut() != null ? (user.getIsLockedOut() ? 1 : 0) : null);
        vo.setAccessFailedCount(user.getAccessFailedCount());
        vo.setLockoutEnd(user.getLockoutEnd());
        vo.setCreatedAt(user.getCreatedAt());
        vo.setUpdatedAt(user.getUpdatedAt());
        if (practitioner != null) {
            vo.setPhone(practitioner.getPhone());
            vo.setGenderEnum(practitioner.getGender() != null ? practitioner.getGender().getCode() : null);
            vo.setBirthDate(practitioner.getBirthDate());
            vo.setAddress(practitioner.getAddressStreet());
            vo.setYbNo(practitioner.getYbNo());
            vo.setSignature(practitioner.getSignature());
            vo.setDrProfttlCode(practitioner.getDrProfttlCode() != null ? practitioner.getDrProfttlCode().toString() : null);
            vo.setPharPracCertNo(practitioner.getPharPracCertNo());
            vo.setDefaultDeptName(defaultDeptName);
            // 查 adm_pr_assign → 出诊科室 / 号源 / 病区 / 药房 / 库房
            List<PrAssign> assignments = prAssignMapper.selectList(
                    new LambdaQueryWrapper<PrAssign>().eq(PrAssign::getPractitionerId, practitioner.getId()));
            // 出诊科室
            Set<String> orgIds = assignments.stream().map(PrAssign::getOrgId).filter(Objects::nonNull).collect(Collectors.toSet());
            vo.setConsultDeptIds(new ArrayList<>(orgIds));
            vo.setConsultDeptNames(orgIds.isEmpty() ? Collections.emptyList() :
                    orgMapper.selectBatchIds(orgIds).stream().filter(o -> o.getIsDeleted() == 0).map(AdmOrganization::getName).collect(Collectors.toList()));
            // 号源
            List<String> assignIds = assignments.stream().map(PrAssign::getId).collect(Collectors.toList());
            Set<String> svcIds = assignIds.isEmpty() ? Collections.emptySet() :
                    prAssignServiceMapper.selectList(new LambdaQueryWrapper<PrAssignService>().in(PrAssignService::getAssignmentId, assignIds))
                            .stream().map(PrAssignService::getHealthcareServiceId).filter(Objects::nonNull).collect(Collectors.toSet());
            vo.setReceptionSourceIds(new ArrayList<>(svcIds));
            vo.setReceptionSourceNames(svcIds.isEmpty() ? Collections.emptyList() :
                    healthcareServiceMapper.selectBatchIds(svcIds).stream().filter(s -> s.getIsDeleted() == 0).map(AdmHealthcareService::getTypeCode).collect(Collectors.toList()));
            // 位置 → 按 form_enum 分三组
            List<String> wIds = new ArrayList<>(), wNames = new ArrayList<>();
            List<String> pIds = new ArrayList<>(), pNames = new ArrayList<>();
            List<String> whIds = new ArrayList<>(), whNames = new ArrayList<>();
            Set<String> locIds = assignments.stream().map(PrAssign::getLocationId).filter(Objects::nonNull).collect(Collectors.toSet());
            if (!locIds.isEmpty()) {
                locationMapper.selectBatchIds(locIds).stream().filter(l -> l.getIsDeleted() == 0).forEach(loc -> {
                    if ("WARD".equals(loc.getFormEnum())) { wIds.add(loc.getId()); wNames.add(loc.getName()); }
                    else if ("PHARMACY".equals(loc.getFormEnum())) { pIds.add(loc.getId()); pNames.add(loc.getName()); }
                    else if ("WAREHOUSE".equals(loc.getFormEnum())) { whIds.add(loc.getId()); whNames.add(loc.getName()); }
                });
            }
            vo.setWardIds(wIds); vo.setWardNames(wNames);
            vo.setPharmacyIds(pIds); vo.setPharmacyNames(pNames);
            vo.setWarehouseIds(whIds); vo.setWarehouseNames(whNames);
        } else {
            vo.setConsultDeptIds(Collections.emptyList()); vo.setConsultDeptNames(Collections.emptyList());
            vo.setReceptionSourceIds(Collections.emptyList()); vo.setReceptionSourceNames(Collections.emptyList());
            vo.setWardIds(Collections.emptyList()); vo.setWardNames(Collections.emptyList());
            vo.setPharmacyIds(Collections.emptyList()); vo.setPharmacyNames(Collections.emptyList());
            vo.setWarehouseIds(Collections.emptyList()); vo.setWarehouseNames(Collections.emptyList());
        }
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean updateUser(UserUpdateDTO dto) {
        // ========== 1. 校验用户存在 ==========
        // 由于 @TableLogic 会自动过滤 is_deleted=0，selectById 只会查未删除记录
        User user = baseMapper.selectById(dto.getId());
        if (user == null) {
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }

        // 获取当前操作人及时间
        String currentUserId = getCurrentUserId();
        LocalDateTime now = LocalDateTime.now();

        // ========== 2. 更新 whale_users 主表 ==========
        user.setUserName(dto.getUsername());
        user.setEmail(dto.getEmail());
        user.setPhoneNumber(dto.getPhoneNumber());
        user.setName(dto.getName());
        user.setIsActive(dto.getIsActive() != null ? dto.getIsActive() == 1 : null);
        baseMapper.updateById(user);

        // ========== 3. 更新 adm_practitioner ==========
        Practitioner practitioner = practitionerMapper.selectOne(
                new LambdaQueryWrapper<Practitioner>()
                        .eq(Practitioner::getUserId, dto.getId())
                        .eq(Practitioner::getIsDeleted, 0) // 只查未删除
        );
        if (practitioner == null) {
            practitioner = new Practitioner();
            practitioner.setUserId(dto.getId());
        }

        // 处理默认科室
        String defaultDeptId = "";
        if (StringUtils.hasText(dto.getDefaultOrgId())) {
            defaultDeptId = dto.getDefaultOrgId();
        } else {
            Practitioner currentPractitioner = practitionerMapper.selectOne(
                    new LambdaQueryWrapper<Practitioner>()
                            .eq(Practitioner::getUserId, currentUserId)
                            .eq(Practitioner::getIsDeleted, 0)
            );
            if (currentPractitioner != null && currentPractitioner.getOrgId() != null) {
                defaultDeptId=currentPractitioner.getOrgId();
            }
        }

        practitioner.setPhone(dto.getPhoneNumber());
        practitioner.setGender(dto.getGenderEnum());
        practitioner.setBirthDate(dto.getBirthDate());
        practitioner.setAddressStreet(dto.getAddress());
        practitioner.setYbNo(dto.getYbNo());
        practitioner.setSignature(dto.getSignature());
        practitioner.setDrProfttlCode(StringUtils.hasText(dto.getDrProfttlCode()) ? Integer.valueOf(dto.getDrProfttlCode()) : null);
        practitioner.setPharPracCertNo(dto.getPharPracCertNo());
        practitioner.setOrgId(defaultDeptId);

        if (practitioner.getId() == null) {
            practitionerMapper.insert(practitioner);
        } else {
            practitionerMapper.updateById(practitioner);
        }

        // ========== 4. 更新用户角色关联：只删变动的，只插新增的 ==========
        // 4.1 查询旧角色
        List<UserRole> oldUserRoles = userRoleMapper.selectList(
                new LambdaQueryWrapper<UserRole>()
                        .eq(UserRole::getUserId, dto.getId())
                        .eq(UserRole::getIsDeleted, 0)
        );
        Set<String> oldRoleIdSet = oldUserRoles.stream().map(UserRole::getRoleId).collect(Collectors.toSet());
        Set<String> newRoleIdSet = dto.getRoleIds() != null ? new HashSet<>(dto.getRoleIds()) : Collections.emptySet();

        // 4.2 软删除"旧有但新没有"的角色
        List<UserRole> toDelete = oldUserRoles.stream()
                .filter(ur -> !newRoleIdSet.contains(ur.getRoleId()))
                .peek(ur -> { ur.setDeletedBy(currentUserId); ur.setDeletedAt(now); })
                .collect(Collectors.toList());
        if (!toDelete.isEmpty()) {
            for (UserRole ur : toDelete) {
                userRoleMapper.updateById(ur);
            }
            userRoleMapper.deleteBatchIds(toDelete.stream().map(UserRole::getId).collect(Collectors.toList()));
        }

        // 4.3 插入"新有但旧没有"的角色
        Set<String> toInsert = new HashSet<>(newRoleIdSet);
        toInsert.removeAll(oldRoleIdSet);
        for (String roleId : toInsert) {
            UserRole ur = new UserRole();
            ur.setUserId(dto.getId());
            ur.setRoleId(roleId);
            userRoleMapper.insert(ur);
        }

        // ========== 5. 查询新角色的 code 映射 ==========
        Map<String, String> roleCodeMap = dto.getRoleIds() == null || dto.getRoleIds().isEmpty()
                ? Collections.emptyMap()
                : roleMapper.selectBatchIds(dto.getRoleIds()).stream()
                .filter(r -> r.getIsDeleted() == 0)
                .collect(Collectors.toMap(RoleDO::getId, r -> r.getCode() != null ? r.getCode() : ""));

        // ========== 6. 处理权限分配（pr_assign 及 pr_assign_service） ==========
        // 6.1 查询该 practitioner 下所有未删除的旧权限分配
        List<PrAssign> oldAssigns = prAssignMapper.selectList(
                new LambdaQueryWrapper<PrAssign>()
                        .eq(PrAssign::getPractitionerId, practitioner.getId())
                        .eq(PrAssign::getIsDeleted, 0)
        );
        if (!oldAssigns.isEmpty()) {
            // 收集所有 assignment_id
            List<String> assignIds = oldAssigns.stream().map(PrAssign::getId).collect(Collectors.toList());

            // 6.2 先处理子表 pr_assign_service
            List<PrAssignService> oldServices = prAssignServiceMapper.selectList(
                    new LambdaQueryWrapper<PrAssignService>()
                            .in(PrAssignService::getAssignmentId, assignIds)
                            .eq(PrAssignService::getIsDeleted, 0)
            );
            if (!oldServices.isEmpty()) {
                // 填充删除信息
                for (PrAssignService service : oldServices) {
                    service.setDeletedBy(currentUserId);
                    service.setDeletedAt(now);
                    prAssignServiceMapper.updateById(service);
                }
                List<String> serviceIds = oldServices.stream().map(PrAssignService::getId).collect(Collectors.toList());
                prAssignServiceMapper.deleteBatchIds(serviceIds);
            }

            // 6.3 再处理父表 pr_assign
            for (PrAssign pa : oldAssigns) {
                pa.setDeletedBy(currentUserId);
                pa.setDeletedAt(now);
                prAssignMapper.updateById(pa);
            }
            prAssignMapper.deleteBatchIds(assignIds);
        }

        // ========== 7. 按新角色重建权限分配 ==========
        if (!roleCodeMap.isEmpty()) {
            // 医生 → 出诊科室 + 号源
            if (roleCodeMap.containsValue("ROLE_DOCTOR")) {
                String roleId = getKeyByValue(roleCodeMap, "ROLE_DOCTOR");
                if (dto.getConsultDeptIds() != null) {
                    for (String orgId : dto.getConsultDeptIds()) {
                        PrAssign pa = newPrAssign(practitioner.getId(), roleId, orgId, null);
                        prAssignMapper.insert(pa);
                    }
                }
                if (dto.getReceptionSourceIds() != null) {
                    String orgId = (dto.getConsultDeptIds() != null && !dto.getConsultDeptIds().isEmpty())
                            ? dto.getConsultDeptIds().get(0)
                            : practitioner.getOrgId();
                    for (String svcId : dto.getReceptionSourceIds()) {
                        PrAssign pa = newPrAssign(practitioner.getId(), roleId, orgId, null);
                        prAssignMapper.insert(pa);
                        PrAssignService pas = new PrAssignService();
                        pas.setAssignmentId(pa.getId());
                        pas.setHealthcareServiceId(svcId);
                        prAssignServiceMapper.insert(pas);
                    }
                }
            }
            // 护士 → 病区
            if (roleCodeMap.containsValue("ROLE_NURSE") && dto.getWardIds() != null) {
                String roleId = getKeyByValue(roleCodeMap, "ROLE_NURSE");
                for (String locId : dto.getWardIds()) {
                    prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                }
            }
            // 药师 → 药房
            if (roleCodeMap.containsValue("ROLE_PHARMACIST") && dto.getPharmacyIds() != null) {
                String roleId = getKeyByValue(roleCodeMap, "ROLE_PHARMACIST");
                for (String locId : dto.getPharmacyIds()) {
                    prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                }
            }
            // 库管 → 库房 + 药房
            if (roleCodeMap.containsValue("ROLE_WAREHOUSE")) {
                String roleId = getKeyByValue(roleCodeMap, "ROLE_WAREHOUSE");
                if (dto.getWarehouseIds() != null) {
                    for (String locId : dto.getWarehouseIds()) {
                        prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                    }
                }
                if (dto.getPharmacyIds() != null) {
                    for (String locId : dto.getPharmacyIds()) {
                        prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                    }
                }
            }
        }

        // ========== 8. 更新用户-组织单元关联 ==========
        UserOrganizationUnit existingUnit = userOrganizationUnitMapper.selectOne(
                new LambdaQueryWrapper<UserOrganizationUnit>()
                        .eq(UserOrganizationUnit::getUserId, dto.getId())
                        .eq(UserOrganizationUnit::getIsDeleted, 0)
        );
        if (StringUtils.hasText(defaultDeptId)) {
            if (existingUnit != null) {
                // 如果科室ID发生变化，则更新；否则不变
                if (!existingUnit.getOrganizationUnitId().equals(defaultDeptId)) {
                    existingUnit.setOrganizationUnitId(defaultDeptId);
                    userOrganizationUnitMapper.updateById(existingUnit);
                }
            } else {
                UserOrganizationUnit newUnit = new UserOrganizationUnit();
                newUnit.setUserId(dto.getId());
                newUnit.setOrganizationUnitId(defaultDeptId);
                userOrganizationUnitMapper.insert(newUnit);
            }
        }
        return true;
    }

    private PrAssign newPrAssign(String practitionerId, String roleId, String orgId, String locationId) {
        PrAssign pa = new PrAssign();
        pa.setPractitionerId(practitionerId);
        pa.setRoleId(roleId);
        pa.setOrgId(orgId);
        pa.setLocationId(locationId);
        return pa;
    }

    private String getKeyByValue(Map<String, String> map, String value) {
        return map.entrySet().stream().filter(e -> value.equals(e.getValue())).map(Map.Entry::getKey).findFirst().orElse(null);
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
            throw new SystemException(UserStatus.GET_USER_FAILED);
        }
    }
}