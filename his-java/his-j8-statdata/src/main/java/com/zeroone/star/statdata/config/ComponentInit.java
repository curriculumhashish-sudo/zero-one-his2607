package com.zeroone.star.statdata.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.statdata.filter.UserHeaderDecodeFilter;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.Ordered;

import jakarta.servlet.DispatcherType;

/**
 * <p>
 * 描述：初始化自定义组件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.exception",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.components.user"
})
public class ComponentInit {
    @Bean
    public EhResultWrapper ehResultWrapper() {
        return JsonVO::create;
    }

    @Bean
    public FilterRegistrationBean<UserHeaderDecodeFilter> userHeaderDecodeFilterRegistration() {
        FilterRegistrationBean<UserHeaderDecodeFilter> registration = new FilterRegistrationBean<>();
        registration.setFilter(new UserHeaderDecodeFilter());
        registration.addUrlPatterns("/*");
        registration.setName("userHeaderDecodeFilter");
        registration.setOrder(Ordered.HIGHEST_PRECEDENCE);
        registration.setDispatcherTypes(DispatcherType.REQUEST, DispatcherType.FORWARD, DispatcherType.INCLUDE, DispatcherType.ERROR);
        return registration;
    }
}