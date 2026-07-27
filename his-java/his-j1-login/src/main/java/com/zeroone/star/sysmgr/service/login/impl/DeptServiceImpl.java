package com.zeroone.star.sysmgr.service.login.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.query.j1.login.DeptQuery;
import com.zeroone.star.project.vo.j1.login.DepartmentVO;
import com.zeroone.star.sysmgr.entity.login.WhaleOrganizationUnitDO;
import com.zeroone.star.sysmgr.mapper.login.DeptMapper;
import com.zeroone.star.sysmgr.service.login.DeptService;
import jakarta.annotation.Resource;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;
@Service
public class DeptServiceImpl extends ServiceImpl<DeptMapper, WhaleOrganizationUnitDO> implements DeptService {

    @Resource
    private UserHolder userholder;
    private RedisTemplate<String, Object> redisTemplate;

    @Override
    public List<DepartmentVO> deptList(DeptQuery query) {
        try {
            UserDTO user = userholder.getCurrentUser();
            String userId = user.getId();
            String tenantId = query.getTenantId();
            List<String> roles = user.getRoles();
            if (!roles.contains("ROLE_DOCTOR")) {
                return Collections.emptyList();
            }
            List<WhaleOrganizationUnitDO> doList = baseMapper.selectUserDeptList(userId, tenantId);
            return BeanUtil.copyToList(doList, DepartmentVO.class);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }


}
