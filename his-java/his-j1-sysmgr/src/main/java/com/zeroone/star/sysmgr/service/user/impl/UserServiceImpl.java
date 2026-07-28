package com.zeroone.star.sysmgr.service.user.impl;

import cn.hutool.crypto.digest.BCrypt;
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
import com.zeroone.star.sysmgr.entity.resource.AdmLocation;
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
    private IPractitionerService practitionerService;
    @Resource
    private IUserOrganizationUnitService userOrganizationUnitService;
    @Resource
    private IUserRoleService userRoleService;
    @Resource
    private IUserTenantService userTenantService;
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

    @Override
    public boolean softDeleteById(String id){
        //1.查询用户
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        //2.获取删除用户用户id
        String deletedBy=getCurrentUserId();
        //3.删除用户
        return baseMapper.softDeleteById(id, deletedBy) > 0;
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

    @Transactional(rollbackFor = Exception.class)
    @Override
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
        practitioner.setDrProfttlCode(userAddDTO.getDrProfttlCode() != null ? Integer.valueOf(userAddDTO.getDrProfttlCode()) : null);
        practitioner.setPharPracCertNo(userAddDTO.getPharPracCertNo());
        practitioner.setOrgId(userAddDTO.getDefaultOrgId());
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
        userOrganizationUnitService.add(userId, userAddDTO.getDefaultOrgId());
        // 8.关联租户
        userTenantService.add(userId);
        return true;
    }

    @Override
    public PageDTO<UserPageVO> getPage(UserQuery query) {
        // 1.分页参数默认值
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        long pageSize  = query.getPageSize()  > 0 ? query.getPageSize()  : 10;

        // 2.构建查询条件：查询 whale_users 表（is_deleted 由 @TableLogic 自动过滤）
        QueryWrapper<User> wrapper = new QueryWrapper<>();
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
                : roleMapper.selectBatchIds(roleIds).stream().filter(r -> r.getIsDeleted() == 0).collect(Collectors.toList());
        // 4.默认科室名称
        String defaultDeptNames = "";
        if (practitioner != null && practitioner.getOrgId() != null) {
            AdmOrganization org = orgMapper.selectById(practitioner.getOrgId());
            if (org != null) defaultDeptNames = org.getName();
        }
        // 5.组装返回
        UserDetailVO vo = new UserDetailVO();
        vo.setId(user.getId());
        vo.setUsername(user.getUserName());
        vo.setName(user.getName());
        vo.setEmail(user.getEmail());
        vo.setRoleName(roles.stream().map(RoleDO::getDisplayName).collect(Collectors.joining(",")));
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
            vo.setDefaultDeptNames(defaultDeptNames);
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
    public Boolean updateUser(UserUpdateDTO dto) {
        // 1.校验用户存在
        User user = baseMapper.selectById(dto.getId());
        if (user == null) {
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        // 2.更新 whale_users
        user.setUserName(dto.getUsername());
        user.setEmail(dto.getEmail());
        user.setPhoneNumber(dto.getPhoneNumber());
        user.setName(dto.getName());
        user.setIsActive(dto.getIsActive() != null ? dto.getIsActive() == 1 : null);
        baseMapper.updateById(user);

        // 3.更新 adm_practitioner
        Practitioner practitioner = practitionerMapper.selectOne(
                new LambdaQueryWrapper<Practitioner>().eq(Practitioner::getUserId, dto.getId()));
        if (practitioner == null) {
            practitioner = new Practitioner();
            practitioner.setUserId(dto.getId());
        }
        practitioner.setPhone(dto.getPhoneNumber());
        practitioner.setGender(dto.getGenderEnum());
        practitioner.setBirthDate(dto.getBirthDate());
        practitioner.setAddressStreet(dto.getAddress());
        practitioner.setYbNo(dto.getYbNo());
        practitioner.setSignature(dto.getSignature());
        practitioner.setDrProfttlCode(dto.getDrProfttlCode() != null ? Integer.valueOf(dto.getDrProfttlCode()) : null);
        practitioner.setPharPracCertNo(dto.getPharPracCertNo());
        practitioner.setOrgId(dto.getDefaultOrgId());
        if (practitioner.getId() == null) {
            practitionerMapper.insert(practitioner);
        } else {
            practitionerMapper.updateById(practitioner);
        }

        // 4.更新角色关联：删旧插新
        userRoleService.remove(new LambdaQueryWrapper<UserRole>().eq(UserRole::getUserId, dto.getId()));
        if (dto.getRoleIds() != null && !dto.getRoleIds().isEmpty()) {
            for (String roleId : dto.getRoleIds()) {
                UserRole ur = new UserRole();
                ur.setUserId(dto.getId());
                ur.setRoleId(roleId);
                userRoleService.save(ur);
            }
        }

        // 5.查询新角色的 code 和 roleId 映射
        Map<String, String> roleCodeMap = dto.getRoleIds() == null || dto.getRoleIds().isEmpty()
                ? Collections.emptyMap()
                : roleMapper.selectBatchIds(dto.getRoleIds()).stream()
                        .filter(r -> r.getIsDeleted() == 0)
                        .collect(Collectors.toMap(RoleDO::getId, r -> r.getCode() != null ? r.getCode() : ""));

        // 6.全删旧 adm_pr_assign + adm_pr_assign_service，按新角色重建
        List<PrAssign> oldAssigns = prAssignMapper.selectList(
                new LambdaQueryWrapper<PrAssign>().eq(PrAssign::getPractitionerId, practitioner.getId()));
        for (PrAssign old : oldAssigns) {
            prAssignServiceMapper.delete(new LambdaQueryWrapper<PrAssignService>().eq(PrAssignService::getAssignmentId, old.getId()));
            prAssignMapper.deleteById(old.getId());
        }
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
            // 护士 → 病区 (WARD)
            if (roleCodeMap.containsValue("ROLE_NURSE") && dto.getWardIds() != null) {
                String roleId = getKeyByValue(roleCodeMap, "ROLE_NURSE");
                for (String locId : dto.getWardIds()) {
                    prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                }
            }
            // 药师 → 药房 (PHARMACY)
            if (roleCodeMap.containsValue("ROLE_PHARMACIST") && dto.getPharmacyIds() != null) {
                String roleId = getKeyByValue(roleCodeMap, "ROLE_PHARMACIST");
                for (String locId : dto.getPharmacyIds()) {
                    prAssignMapper.insert(newPrAssign(practitioner.getId(), roleId, null, locId));
                }
            }
            // 库管 → 库房 (WAREHOUSE) + 药房 (PHARMACY)
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

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user.getId();
        } catch (Exception e) {
            throw new SystemException(UserStatus.GET_USER_FAILED);
        }
    }
}
