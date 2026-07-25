package com.zeroone.star.sysmgr.config;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.redis",
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.components.user"
})
public class ComponentInit {
}
