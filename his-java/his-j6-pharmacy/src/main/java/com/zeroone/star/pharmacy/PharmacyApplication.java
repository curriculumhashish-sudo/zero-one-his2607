package com.zeroone.star.pharmacy;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：药库管理模块服务启动入口
 * </p>
 * @author AB
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class PharmacyApplication {

    public static void main(String[] args) {
        SpringApplication.run(PharmacyApplication.class, args);
    }

}
