package com.zeroone.star.j7.pharmacy;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：药房管理模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class J7PharmacyApplication {
    public static void main(String[] args) {
        SpringApplication.run(J7PharmacyApplication.class, args);
    }
}
