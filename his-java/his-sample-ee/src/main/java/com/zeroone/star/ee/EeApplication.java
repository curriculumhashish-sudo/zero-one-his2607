package com.zeroone.star.ee;

import org.dromara.easyes.spring.annotation.EsMapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：EE 模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
@EsMapperScan("com.zeroone.star.ee.mapper")
public class EeApplication {
    public static void main(String[] args) {
        SpringApplication.run(EeApplication.class, args);
    }
}
