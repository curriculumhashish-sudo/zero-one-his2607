package com.zeroone.star.j7.dispense.config;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan("com.zeroone.star.project.config.mybatis")
//@MapperScan("com.zeroone.star.j7.dispense.mapper")
public class MyBatisInit {
}
