package com.zeroone.star.workbench.service.impl;


import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
// import com.zeroone.star.project.constant.RedisConstant;
import com.zeroone.star.project.query.j2.workbench.UserWorkbenchQuery;
import com.zeroone.star.project.vo.j2.workbench.UserWorkbenchVO;
import com.zeroone.star.workbench.entity.WhaleMenus;
import com.zeroone.star.workbench.mapper.WhaleMenusMapper;
import com.zeroone.star.workbench.mapper.WhaleRolesMapper;
import com.zeroone.star.workbench.service.IWorkbenchService;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
// import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
// import org.springframework.util.StringUtils;

import java.util.Collections;
import java.util.List;
// import java.util.concurrent.TimeUnit;

/**
 * <p>
 * 工作台业务服务实现类
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Slf4j
@Service
public class WorkbenchServiceImpl extends ServiceImpl<WhaleMenusMapper, WhaleMenus> implements IWorkbenchService {

    @Resource
    private WhaleRolesMapper whaleRolesMapper;

    @Resource
    private UserHolder userHolder;

    // @Resource
    // private RedisTemplate<String, Object> redisTemplate;

    // private static final long CACHE_TTL_MINUTES = 30;

    @Override
    public List<UserWorkbenchVO> queryUserWorkbench(UserWorkbenchQuery query) {
        String userId;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            userId = currentUser.getId();
        } catch (Exception e) {
            log.warn("JWT解析失败，降级使用query中的userId", e);
            userId = query.getUserId();
        }
        if (userId == null || userId.isEmpty()) {
            userId = query.getUserId();
        }

        String tenantId = query.getTenantId();
        String keyword = query.getKeyword();

        // boolean useCache = !StringUtils.hasText(keyword);
        // if (useCache) {
        //     String cacheKey = RedisConstant.WORKBENCH_MENU_PREFIX + tenantId + ":" + userId;
        //     Object cached = redisTemplate.opsForValue().get(cacheKey);
        //     if (cached instanceof List<?> cacheList) {
        //         log.debug("命中工作台缓存, userId={}, tenantId={}", userId, tenantId);
        //         return (List<UserWorkbenchVO>) cacheList;
        //     }
        // }

        List<UserWorkbenchVO> result = whaleRolesMapper.selectUserWorkbench(tenantId, userId, keyword);
        if (result == null || result.isEmpty()) {
            log.info("用户[{}]在租户[{}]下未查询到工作台菜单数据", userId, tenantId);
            return Collections.emptyList();
        }

        // if (useCache) {
        //     String cacheKey = RedisConstant.WORKBENCH_MENU_PREFIX + tenantId + ":" + userId;
        //     redisTemplate.opsForValue().set(cacheKey, result, CACHE_TTL_MINUTES, TimeUnit.MINUTES);
        //     log.debug("工作台数据已缓存, userId={}, tenantId={}, TTL={}min", userId, tenantId, CACHE_TTL_MINUTES);
        // }

        return result;
    }
}
