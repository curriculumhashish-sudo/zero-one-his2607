package com.zeroone.star.configuration.config;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@MapperScan({
        "com.zeroone.star.configuration.mapper",
        "com.zeroone.star.configuration.orderset.mapper"
})
public class MyBatisInit {
}
