package com.zeroone.star.sysmgr.service.dept.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.collection.CollUtil;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.constant.RedisConstant;
import com.zeroone.star.project.dto.j1.login.DeptSelectDTO;
import com.zeroone.star.project.query.j1.login.DeptQuery;
import com.zeroone.star.project.vo.j1.login.DepartmentVO;
import com.zeroone.star.project.vo.j1.login.DeptBindResultVO;
import com.zeroone.star.sysmgr.constant.DeptRedisConstant;
import com.zeroone.star.sysmgr.entity.dept.WhaleOrganizationUnitDO;
import com.zeroone.star.sysmgr.mapper.dept.DeptMapper;
import com.zeroone.star.sysmgr.mapper.login.LoginSessionMapper;
import com.zeroone.star.sysmgr.service.dept.DeptService;
import jakarta.annotation.Resource;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

@Service
public class DeptServiceImpl implements DeptService {

    @Resource
    private UserHolder userholder;
    @Resource
    private DeptMapper deptmapper;
    @Resource
    private LoginSessionMapper loginSessionMapper;
    @Resource
    private RedisTemplate<String, Object> redisTemplate;
    private static final String ROLE_DOCTOR = "ROLE_DOCTOR";

    @Override
    public List<DepartmentVO> deptList(DeptQuery query) {
        // 医生角色校验
        checkDoctorRole();

        try {
            UserDTO user = userholder.getCurrentUser();
            String userId = user.getId();
            // 查询租户
            String tenantId = getTenantIdByUserId(userId);

            String cacheKey = DeptRedisConstant.getUserAllowDeptKey(tenantId, userId);
            List<String> cacheDeptIdList = (List<String>) redisTemplate.opsForValue().get(cacheKey);

            if (CollUtil.isNotEmpty(cacheDeptIdList)) {
                return cacheDeptIdList.stream().map(id -> {
                    DepartmentVO vo = new DepartmentVO();
                    vo.setId(id);
                    return vo;
                }).collect(Collectors.toList());
            }

            List<WhaleOrganizationUnitDO> doList = deptmapper.selectUserDeptList(userId, tenantId);
            List<DepartmentVO> voList = BeanUtil.copyToList(doList, DepartmentVO.class);
            List<String> deptIdList = voList.stream()
                    .map(DepartmentVO::getId)
                    .collect(Collectors.toList());

            redisTemplate.opsForValue().set(
                    cacheKey,
                    deptIdList,
                    DeptRedisConstant.USER_ALLOW_DEPT_CACHE_TTL,
                    TimeUnit.SECONDS
            );
            return voList;

        } catch (Exception e) {
            throw new RuntimeException(e);
        }


    }

    @Override
    public DeptBindResultVO selectDept(DeptSelectDTO dto) {
        // 1. 医生角色校验
        checkDoctorRole();

        // 2. 获取当前登录用户ID
        try{
            String userId = userholder.getCurrentUser().getId();
            String selectDeptId = dto.getOrganizationUnitId();

            // 查询当前用户允许选择的科室列表
            DeptQuery query = new DeptQuery();
            List<DepartmentVO> allowDeptList = deptList(query);

            // 越权校验：选中科室必须在可选范围
            boolean isValid = allowDeptList.stream()
                    .anyMatch(vo -> selectDeptId.equals(vo.getId()));
            if (!isValid) {
                throw new RuntimeException("所选科室不在权限范围内");
            }

            // 更新数据库会话表选中科室
            loginSessionMapper.updateSelectOrgUnitId(userId, selectDeptId);

            // 同步更新Redis会话缓存
            String sessionRedisKey = "user:session:" + userId;
            redisTemplate.opsForHash().put(sessionRedisKey, "organizationUnitId", selectDeptId);

            // 组装返回结果
            DeptBindResultVO vo = new DeptBindResultVO();
            vo.setOrganizationUnitId(selectDeptId);
            return vo;
        }catch (Exception e){
            throw new RuntimeException(e);
        }

    }


    /**
     * 校验当前登录账号是否为医生角色，非医生直接抛出异常
     */
    private void checkDoctorRole() {
        UserDTO user = null;
        try {
            user = userholder.getCurrentUser();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        List<String> roles = user.getRoles();
        if (!roles.contains(ROLE_DOCTOR)) {
            throw new RuntimeException("");
        }
    }

    /**
     * 根据userId从会话Hash获取租户ID
     * key = user:session:{userId}  hashKey: tenant_id
     */
    private String getTenantIdByUserId(String userId) {
        String sessionKey = RedisConstant.USER_SESSION_PREFIX + userId;
        Object tenantObj = redisTemplate.opsForHash().get(sessionKey, "tenant_id");
        if (tenantObj == null) {
            throw new RuntimeException("会话租户信息缺失，请重新登录");
        }
        return tenantObj.toString();
    }



}
