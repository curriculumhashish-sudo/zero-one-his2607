package com.zeroone.star.sysmgr.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.redis",
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.components.user",
        "com.zeroone.star.project.exception"
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
