package com.zeroone.star.oauth2.service.impl.oauth2;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.zeroone.cloud.oauth2.entity.SecurityUser;
import com.zeroone.cloud.starter.oauth2.service.LoadUserDetailService;
import com.zeroone.star.oauth2.config.Oauth2Properties;
import com.zeroone.star.oauth2.entity.Practitioner;
import com.zeroone.star.oauth2.entity.Role;
import com.zeroone.star.oauth2.entity.User;
import com.zeroone.star.oauth2.entity.UserOrganizationUnit;
import com.zeroone.star.oauth2.mapper.PractitionerMapper;
import com.zeroone.star.oauth2.mapper.UserOrganizationUnitMapper;
import com.zeroone.star.oauth2.service.IRoleService;
import com.zeroone.star.oauth2.service.IUserService;
import jakarta.annotation.Resource;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：加载用户信息服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Service
public class LoadUserDetailServiceImpl implements LoadUserDetailService {

    @Resource
    Oauth2Properties properties;

    @Override
    public SecurityUser loadUserDetail(String username, String clientId) throws UsernameNotFoundException {
        if (properties.getMgrId().equals(clientId)) {
            return loadUserDetailForMgr(username);
        } else if (properties.getUserId().equals(clientId)) {
            return loadUserDetailForUser(username);
        }
        throw new UsernameNotFoundException("登录客户端ID错误");
    }

    @Resource
    IUserService userService;
    @Resource
    IRoleService roleService;
    @Resource
    PractitionerMapper practitionerMapper;
    @Resource
    UserOrganizationUnitMapper userOrganizationUnitMapper;

    private SecurityUser loadUserDetailForMgr(String username) throws UsernameNotFoundException {
        // TODO：通过用户名查询用户,需要根据你的数据库设计来修改代码
        // 1 通过用户名查找用户对象
        // 注意：whale_users 中同一 user_name 可能存在多条历史重复数据，
        // getOne(wrapper, false) 取第一条且不抛 TooManyResultsException，避免登录 500。
        User user = new User();
        user.setUsername(username);
        user = userService.getOne(new QueryWrapper<>(user), false);
        if (user == null) {
            throw new UsernameNotFoundException("用户名或密码错误");
        }
        // SecurityConfig 注入的是 BCryptPasswordEncoder，必须传入原始 BCrypt 哈希。
        // 不能加 {bcrypt} 前缀；该前缀仅适用于 DelegatingPasswordEncoder，
        // 否则表单登录会把所有正确密码判定为错误。
        // TODO：通过用户编号查询角色,需要根据你的数据库设计来修改代码
        // 2 通过用户ID获取角色列表
        List<Role> roles = roleService.listRoleByUserId(user.getId());

        // 4 加载医务人员信息与组织单元
        // 同一 user_id 在 adm_practitioner 中可能有多条记录，取第一条（selectOne 会抛异常）
        String uid = user.getId();
        Practitioner practitioner = practitionerMapper.selectList(
                        new QueryWrapper<Practitioner>().eq("user_id", uid).eq("is_deleted", 0))
                .stream().findFirst().orElse(null);
        if (practitioner != null) {
            user.setPractitionerId(practitioner.getId());
            user.setPractitionerName(practitioner.getName());
            user.setTitleCode(practitioner.getDrProfttlCode());
            // titleName 暂无编码字典表，置空
            user.setTitleName("");
        }
        List<UserOrganizationUnit> units = userOrganizationUnitMapper.selectList(
                new QueryWrapper<UserOrganizationUnit>().eq("user_id", uid));
        Map<String, String> orgUnits = new LinkedHashMap<>();
        for (UserOrganizationUnit u : units) {
            orgUnits.put(u.getOrganizationUnitId(), u.getOrganizationUnitId());
        }
        user.setOrganizationUnits(orgUnits);

        // 3 构建权限角色对象
        return SecurityUser.create(user, user.getUsername(), user.getPassword(), roles.stream().map(Role::getCode).collect(Collectors.toList()));
    }

    private SecurityUser loadUserDetailForUser(String username) throws UsernameNotFoundException {
        // TODO：用户端查找用户尚未实现
        System.out.println(username);
        throw new UsernameNotFoundException("用户端查找用户尚未实现");
    }
}
