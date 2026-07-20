package com.zeroone.star.oauth2.service.impl.oauth2;

import com.zeroone.cloud.oauth2.entity.SecurityUser;
import com.zeroone.cloud.starter.oauth2.service.TokenEnhancerDataService;
import com.zeroone.star.oauth2.entity.User;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;

/**
 * <p>
 * 描述：Token增强数据处理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Service
public class TokenEnhancerDataServiceImpl implements TokenEnhancerDataService {
    @Override
    public Map<String, Object> enhance(SecurityUser securityUser) {
        Map<String, Object> info = new HashMap<>();
        // 如果是管理端数据库用户DO对象
        if (securityUser.getExtendsObject() instanceof User user) {
            // FIXME: 如果要扩展凭证中的负载数据,需要修改这里的代码
            // 把用户ID设置到JWT中
            info.put("id", user.getId());
        }
        return info;
    }
}
