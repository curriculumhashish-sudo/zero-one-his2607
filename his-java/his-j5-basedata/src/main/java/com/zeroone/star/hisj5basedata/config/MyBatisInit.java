package com.zeroone.star.hisj5basedata.config;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * <p>
 * 描述：初始化MyBatis
 * </p>
 * @author keepasdd
 */
@Configuration
@ComponentScan("com.zeroone.star.project.config.mybatis")
@MapperScan("com.zeroone.star.hisj5basedata.mapper")
public class MyBatisInit {
}
