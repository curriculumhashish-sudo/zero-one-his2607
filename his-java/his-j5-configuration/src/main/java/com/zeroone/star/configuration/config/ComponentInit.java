package com.zeroone.star.configuration.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.components.user",
        "com.zeroone.star.project.exception"
})
public class ComponentInit {

    /**
     * 为公共异常处理器提供本服务的统一响应包装方式。
     */
    @Bean
    @ConditionalOnMissingBean(EhResultWrapper.class)
    public EhResultWrapper ehResultWrapper() {
        return (message, status) -> JsonVO.create(null, status.getCode(), message);
    }
}
