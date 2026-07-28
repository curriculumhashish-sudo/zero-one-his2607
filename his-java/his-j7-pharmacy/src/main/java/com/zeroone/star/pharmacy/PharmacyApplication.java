package com.zeroone.star.pharmacy;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.cloud.openfeign.EnableFeignClients;

/**
 * <p>
 * 描述：药房管理模块启动类
 * </p>
 * <p>
 * 功能说明：药房管理微服务的入口类，负责启动 Spring Boot 应用。
 * 该模块提供住院药房和门诊药房的核心业务功能，包括：
 * - 住院发药驳回
 * - 住院退药确认
 * - 门诊拒绝退药
 * 通过 @EnableDiscoveryClient 注册到 Nacos 服务中心。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */

@SpringBootApplication
@EnableDiscoveryClient
@EnableFeignClients
public class PharmacyApplication {
    public static void main(String[] args) {
        SpringApplication.run(PharmacyApplication.class, args);
    }
}
