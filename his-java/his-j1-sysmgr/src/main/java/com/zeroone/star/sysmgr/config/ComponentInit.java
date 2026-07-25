package com.zeroone.star.sysmgr.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * <p>
 * 描述：初始化自定义组件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.exception",
        "com.zeroone.star.project.components.user",
        "com.zeroone.star.project.components.work",
        "com.zeroone.star.project.components.mybaits"
})
public class ComponentInit {

    /**
     * 响应包装器
     * @return 包装器Bean
     */
    @Bean
    @ConditionalOnMissingBean
    public EhResultWrapper ehResultWrapper() {
        return JsonVO::create;
    }
}
