package com.zeroone.star.assistant.config;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * 初始化通用助手依赖的公共组件。
 */
@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.config.filter.mcp"
})
public class ComponentInit {
}
