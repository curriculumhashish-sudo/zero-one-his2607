package com.zeroone.star.gateway.service.gateway;

import cn.hutool.json.JSONUtil;
import com.zeroone.cloud.starter.gateway.service.LoadResourcesData;
import com.zeroone.star.project.constant.RedisConstant;
import jakarta.annotation.Resource;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * <p>
 * 描述：加载资源数据接口实现，一般是从Redis缓存中获取资源数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Service
public class LoadResourcesDataImpl implements LoadResourcesData {
    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    @Override
    @SuppressWarnings("ConstantConditions")
    public List<String> loadRolesByUrlPath(String url) {
        Object obj = redisTemplate.opsForHash().get(RedisConstant.RESOURCE_ROLES_MAP, url);
        if (obj == null) {
            return null;
        }
        // OAUTH2存储时将角色列表序列化为JSON字符串，此处反序列化
        return JSONUtil.toList(String.valueOf(obj), String.class);
    }
}
