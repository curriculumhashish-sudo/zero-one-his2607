package com.zeroone.stat.sysmgr;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：租户用户管理服务
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class SysmgrApplication {
    public static void main(String[] args) {
        SpringApplication.run(SysmgrApplication.class, args);
    }
}
