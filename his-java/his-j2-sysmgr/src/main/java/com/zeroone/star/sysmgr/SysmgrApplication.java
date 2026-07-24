package com.zeroone.star.sysmgr;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.cloud.openfeign.EnableFeignClients;
/**
 * <p>
 * 描述：系统管理模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
@EnableFeignClients
//http://127.0.0.1:10300/j2-sysmgr/doc.html
public class SysmgrApplication {
    public static void main(String[] args) {
        SpringApplication.run(SysmgrApplication.class, args);
    }
}
