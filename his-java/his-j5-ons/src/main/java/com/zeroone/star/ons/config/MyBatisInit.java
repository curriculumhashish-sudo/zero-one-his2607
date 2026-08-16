package com.zeroone.star.ons.config;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@MapperScan("com.zeroone.star.ons.mapper")
public class MyBatisInit {
}