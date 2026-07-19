package com.zeroone.star.configuration.config;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@MapperScan("com.zeroone.star.configuration.mapper")
public class MyBatisInit {
}