package com.zeroone.star.cpp.config;

import feign.RequestInterceptor;
import feign.RequestTemplate;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.context.annotation.Configuration;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

/**
 * <p>
 * 描述：声明式服务配置，主要解决Header传参问题
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Configuration
public class FeignConfiguration implements RequestInterceptor {
    @Override
    public void apply(RequestTemplate requestTemplate) {
        if (RequestContextHolder.getRequestAttributes() instanceof ServletRequestAttributes attributes) {
            HttpServletRequest request = attributes.getRequest();
            // 1. 转发 Authorization 头（用于身份认证）
            String authorization = request.getHeader("Authorization");
            if (StringUtils.hasText(authorization)) {
                requestTemplate.header("Authorization", authorization);
            }

            // 2. 转发自定义的 X-Request-Id 头（用于全链路追踪）
            String requestId = request.getHeader("X-Request-Id");
            if (StringUtils.hasText(requestId)) {
                requestTemplate.header("X-Request-Id", requestId);
            }

            // 3. 根据需要补充其他头
        }
    }
}
